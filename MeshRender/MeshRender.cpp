#include "MeshRender.h"

#include <limits>

namespace FW {

MeshRender::MeshRender(std::string id, fs::path meshFile) : Visualizer(id), m_meshFile(meshFile) {
}

MeshRender::~MeshRender() {
}

void MeshRender::init() {
	addProperties();
	registerEvents();

	m_mesh = std::make_shared<Mesh>();
	if (!Traits::loadFromFile(*m_mesh, m_meshFile.string())) {
		gui()->log()->error("Could not open mesh file \""+m_meshFile.string()+"\"");
	}
	gui()->log()->info("Loaded mesh with "+lexical_cast<std::string>(Traits::numVertices(*m_mesh))+" vertices and "+lexical_cast<std::string>(Traits::numFaces(*m_mesh))+" faces.");

	m_program = std::make_shared<ShaderProgram>();
	m_program->addShaders(std::string(GLSL_PREFIX)+"mesh.vert", std::string(GLSL_PREFIX)+"mesh.frag");
	m_program->link();
	Eigen::Vector3f lightDir(1.f, 1.f, 1.f);
	lightDir.normalize();
	m_program->use();
	m_program->setUniformVec3("lightDir", lightDir.data());

	m_rm = std::make_shared<Rendered::Mesh<Mesh>>(m_mesh, m_program, false);
	
	float clipRangeMin =  std::numeric_limits<float>::infinity();
	float clipRangeMax = -std::numeric_limits<float>::infinity();
	for (auto vertexId : Traits::vertices(*m_mesh)) {
		float vertexHeight = Traits::vertexPosition(*m_mesh, vertexId)[2];
		if (vertexHeight < clipRangeMin) clipRangeMin = vertexHeight;
		if (vertexHeight > clipRangeMax) clipRangeMax = vertexHeight;
	}
	clipRangeMin -= 0.1f;
	clipRangeMax += 0.1f;
	m_clippingHeight = clipRangeMin + 0.5f * (clipRangeMax - clipRangeMin);
	auto clippingHeightRange = gui()->properties()->get<Range>({"SectionRendering", "ClippingHeight"});
	clippingHeightRange->setMin(clipRangeMin);
	clippingHeightRange->setMax(clipRangeMax);
	clippingHeightRange->setValue(m_clippingHeight);
}

void MeshRender::render() {
	m_program->use();
	
	if (m_clipping) {
		Eigen::Vector3f clipNormal = Eigen::Vector3f(0, 0, 1);
		m_program->setUniformVec3("clipNormal", clipNormal.data());
		m_program->setUniformVar1f("clipDistance", m_clippingHeight);
		glEnable(GL_CLIP_DISTANCE0);
	}
	glEnable(GL_DEPTH_TEST);
	
	m_program->setUniformMat4("mvM", fw()->transforms()->modelview().data());
	m_program->setUniformMat4("prM", fw()->transforms()->projection().data());
	m_program->setUniformMat3("nmM", fw()->transforms()->normal().data());
	m_rm->render();
	
	glDisable(GL_DEPTH_TEST);
	if (m_clipping) {
		glDisable(GL_CLIP_DISTANCE0);
	}
}

void MeshRender::addProperties() {
	auto sectionRendering = gui()->properties()->add<Section>("Rendering", "SectionRendering");
	
	sectionRendering->add<Bool>("Smooth normals", "SmoothNormals")->setCallback([&] (bool checked) {m_rm->setSmoothNormals(checked);});
	sectionRendering->add<Bool>("Enable clipping", "EnableClipping")->setCallback([&] (bool checked) {m_clipping = checked;});
	sectionRendering->add<Range>("Clipping height", "ClippingHeight")->setCallback([&] (float value) {m_clippingHeight = value;});
}

void MeshRender::registerEvents() {
}

MeshRender::Factory::Factory() : FW::Factory() {
}

MeshRender::Factory::~Factory() {
}

void MeshRender::Factory::init() {
	auto path = gui()->properties()->add<File>("Input mesh", "path");
	path->setExtensions({"obj", "off"});
}

Visualizer::Ptr MeshRender::Factory::addVisualizer() {
	std::string name = gui()->properties()->get<String>({"__name__"})->value();
	auto path = gui()->properties()->get<File>({"path"})->value();
	MeshRender::Ptr vis(new MeshRender(name, path));
	return std::dynamic_pointer_cast<Visualizer>(vis);
}

} // FW


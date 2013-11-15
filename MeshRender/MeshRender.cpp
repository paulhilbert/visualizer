#include "MeshRender.h"

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
	m_program->setUniformVec3("lightDir", lightDir.data());

	m_rm = std::make_shared<Rendered::Mesh<Mesh>>(m_mesh, m_program, false);
}

void MeshRender::render() {
	m_program->use();
	m_program->setUniformMat4("mvM", fw()->transforms()->modelview().data());
	m_program->setUniformMat4("prM", fw()->transforms()->projection().data());
	m_program->setUniformMat4("nmM", fw()->transforms()->normal().data());
	m_rm->render();
}

void MeshRender::addProperties() {
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


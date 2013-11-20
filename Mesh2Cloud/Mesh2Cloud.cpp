#include "Mesh2Cloud.h"

namespace FW {

Mesh2Cloud::Mesh2Cloud(std::string id, fs::path meshPath) : Visualizer(id), SingleMesh(id, meshPath), m_cloud(new Cloud()) {
}

Mesh2Cloud::~Mesh2Cloud() {
}

void Mesh2Cloud::init() {
	//m_mesh->request_vertex_status();
	//m_mesh->request_face_status();
	//m_mesh->request_vertex_normals();
	//m_mesh->request_face_normals();
	SingleMesh::init();
	m_mesh->update_face_normals();
	addProperties();
	registerEvents();
}

void Mesh2Cloud::render() {
	SingleMesh::render();
	if (m_rc) m_rc->render(fw()->transforms()->modelview(), fw()->transforms()->projection());
}

void Mesh2Cloud::addProperties() {
	auto samples = gui()->properties()->add<Number>("Samples Per Square Unit", "samples");
	samples->setDigits(0);
	samples->setMin(1);
	samples->setMax(100000);
	samples->setValue(100);
	gui()->properties()->add<Button>("Sample", "sample")->setCallback([&] () { auto ns = gui()->properties()->get<Number>({"samples"})->value(); sample(ns); });;

	auto outFile = gui()->properties()->add<File>("Save to: ", "outFile");
	outFile->setMode(File::SAVE);
	outFile->setCallback([&] (fs::path p) {
		pcl::io::savePCDFileASCII(p.string(), *m_cloud);
		gui()->log()->info("Saved pointcloud to: \""+p.string()+"\"");
	});
	outFile->disable();
}

void Mesh2Cloud::registerEvents() {
}

void Mesh2Cloud::sample(int samplesPerSquareUnit) {
	typedef MeshAnalysis<Traits> MA;
	m_rc.reset();
	execute(
		[&] () { 
			MA::sampleOnSurface<Point>(*m_mesh, samplesPerSquareUnit, m_cloud);
		}, 
		[&] () { 
			gui()->log()->info("Sampling finished. Sampled "+lexical_cast<std::string>(m_cloud->size())+" points.");
			m_rc = Rendered::Cloud::Ptr(new Rendered::Cloud(rgbaRed()));
			m_rc->setFromPCLCloud(m_cloud->begin(), m_cloud->end());
			gui()->properties()->get<File>({"outFile"})->enable();
		}
	);
	//MA::sampleOnSurface<Point>(m_mesh, samplesPerSquareUnit, m_cloud);
}

Mesh2Cloud::Factory::Factory() : FW::Factory() {
}

Mesh2Cloud::Factory::~Factory() {
}

void Mesh2Cloud::Factory::init() {
	auto file = gui()->properties()->add<File>("Input Mesh", "meshFile");
	file->setExtensions({"obj"});
}

Visualizer::Ptr Mesh2Cloud::Factory::addVisualizer() {
	std::string name = gui()->properties()->get<String>({"__name__"})->value();
	fs::path meshPath = gui()->properties()->get<File>({"meshFile"})->value();
	Mesh2Cloud::Ptr vis(new Mesh2Cloud(name, meshPath));
	return std::dynamic_pointer_cast<Visualizer>(vis);
}

} // FW


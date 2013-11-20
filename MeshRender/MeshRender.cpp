#include "MeshRender.h"

#include <limits>

namespace FW {

MeshRender::MeshRender(std::string id, fs::path meshFile) : Visualizer(id), SingleMesh(id, meshFile) {
}

MeshRender::~MeshRender() {
}

void MeshRender::init() {
	SingleMesh::init();
	addProperties();
	registerEvents();
}

void MeshRender::render() {
	SingleMesh::render();
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


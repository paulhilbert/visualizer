#include "MergeAndEditClouds.h"

#include <Vis/Colors.h>
using namespace Vis::Colors;

namespace FW {

MergeAndEditClouds::MergeAndEditClouds(std::string id, const GUI::Property::Paths& paths) : Visualizer(id), m_paths(paths), m_cloud(new Cloud()) {
}

MergeAndEditClouds::~MergeAndEditClouds() {
}

void MergeAndEditClouds::init() {
	addProperties();
	registerEvents();

	m_areaSelect = Input::AreaSelect::Ptr(new Input::AreaSelect(fw()));
	m_areaSelect->setStartCallback([&] () { m_selection.clear(); if (m_rendered) m_rendered->clearAnnotations(); });
	m_areaSelect->setStopCallback([&] () {
		if (!m_rendered) return;
		int idx = 0;
		for (const auto& pnt : *m_cloud) {
			if (m_areaSelect->pointInSelection(pnt.getVector3fMap())) m_selection.push_back(idx);
			++idx;
		}
		if (m_selection.size()) m_rendered->annotate(m_selection, "selection")->colorize(rgbaWhite());
	});
	m_areaSelect->setUnselectCallback([&] () { m_selection.clear(); if (m_rendered) m_rendered->clearAnnotations(); });
	m_areaSelect->init(Eigen::Vector4f(0.f, 0.4f, 1.f, 0.4f));


	for (const auto& p : m_paths) {
		if (!fs::exists(p)) {
			gui()->log()->error("File \""+p.string()+"\" does not exist. Skipping this file.");
			continue;
		}
		if (p.extension() != ".pcd") {
			gui()->log()->error("File \""+p.string()+"\" is not a .pcd file. Skipping this file.");
			continue;
		}
		Cloud singleCloud;
		if (pcl::io::loadPCDFile<Point>(p.string(), singleCloud) == -1) {
			gui()->log()->error("Couldn't read file \""+p.string()+"\". Skipping this file.");
			continue;
		}
		gui()->log()->verbose("Loaded point cloud with "+lexical_cast<std::string>(singleCloud.size())+" points.");
		m_cloud->insert(m_cloud->end(), singleCloud.begin(), singleCloud.end());
	}
	gui()->log()->info("Loaded all point clouds. Point count: "+lexical_cast<std::string>(m_cloud->size()));

	m_rendered = Rendered::Cloud::Ptr(new Rendered::Cloud(Eigen::Vector4f(0.4f, 0.4f, 0.4f, 1.f)));
	m_rendered->setFromPCLCloud(m_cloud->begin(), m_cloud->end());
}

void MergeAndEditClouds::render() {
	m_rendered->render(fw()->transforms()->modelview(), fw()->transforms()->projection());
	m_areaSelect->render();
}

void MergeAndEditClouds::addProperties() {
}

void MergeAndEditClouds::registerEvents() {
}

MergeAndEditClouds::Factory::Factory() : FW::Factory() {
}

MergeAndEditClouds::Factory::~Factory() {
}

void MergeAndEditClouds::Factory::init() {
	auto paths = gui()->properties()->add<Files>("Input Files:", "paths");
}

Visualizer::Ptr MergeAndEditClouds::Factory::addVisualizer() {
	std::string name = gui()->properties()->get<String>({"__name__"})->value();
	auto paths = gui()->properties()->get<Files>({"paths"})->value();
	MergeAndEditClouds::Ptr vis(new MergeAndEditClouds(name, paths));
	return std::dynamic_pointer_cast<Visualizer>(vis);
}

} // FW


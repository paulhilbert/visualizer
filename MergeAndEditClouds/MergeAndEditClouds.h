#ifndef MERGEANDEDITCLOUDSVIS_H_
#define MERGEANDEDITCLOUDSVIS_H_

#include <FW/Visualizer.h>
#include <FW/Factory.h>

#include <Geometry/PCLTools.h>
using Geometry::PCLTools;
typedef pcl::PointNormal Point;
typedef PCLTools<Point>  Tools;
typedef Tools::CloudType Cloud;
typedef Tools::Idx       Idx;
typedef Tools::IdxSet    IdxSet;

#include <Library/Rendered/Cloud.h>
#include <Library/Input/AreaSelect.h>


namespace FW {


class MergeAndEditClouds : public Visualizer {
	public:
		class Factory;

	public:
		MergeAndEditClouds(std::string id, const GUI::Property::Paths& paths);
		virtual ~MergeAndEditClouds();

		void init();
		void render();
		void addProperties();
		void registerEvents();

	protected:
		GUI::Property::Paths   m_paths;
		Cloud::Ptr             m_cloud;
		Rendered::Cloud::Ptr   m_rendered;

		// selection
		Input::AreaSelect::Ptr m_areaSelect;
		IdxSet                 m_selection;
};


class MergeAndEditClouds::Factory : public FW::Factory {
	public:
		Factory();
		virtual ~Factory();

		void init();
		Visualizer::Ptr addVisualizer();
};

} // FW

#endif /* MERGEANDEDITCLOUDSVIS_H_ */

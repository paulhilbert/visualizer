#ifndef MESH2CLOUDVIS_H_
#define MESH2CLOUDVIS_H_

#include <FW/FWVisualizer.h>
#include <FW/FWFactory.h>

#include <Geometry/PCLTools.h>
using Geometry::PCLTools;
typedef pcl::PointNormal Point;
typedef PCLTools<Point>  Tools;
typedef Tools::CloudType Cloud;
typedef Tools::Idx       Idx;
typedef Tools::IdxSet    IdxSet;

#include <Library/Rendered/Cloud.h>

#include <Library/Visualizer/SingleMesh/SingleMesh.h>
#include <Library/Visualizer/MultiPointCloud/MultiPointCloud.h>
#include <Library/Geometry/MeshAnalysis.h>


namespace FW {


class Mesh2Cloud : public SingleMesh, public MultiPointCloud {
	public:
		typedef std::shared_ptr<Mesh2Cloud> Ptr;
		typedef std::weak_ptr<Mesh2Cloud>   WPtr;

	public:
		class Factory;

	public:
		Mesh2Cloud(std::string id, fs::path meshPath);
		virtual ~Mesh2Cloud();

		void init();
		void render();
		void addProperties();
		void registerEvents();

	protected:
		void sample(int samplesPerSquareUnit);
};


class Mesh2Cloud::Factory : public FW::Factory {
	public:
		Factory();
		virtual ~Factory();

		void init();
		Visualizer::Ptr addVisualizer();
};


} // FW

#endif /* MESH2CLOUDVIS_H_ */

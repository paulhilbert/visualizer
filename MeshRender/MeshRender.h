#ifndef MESHRENDERVIS_H_
#define MESHRENDERVIS_H_

#include <FW/FWVisualizer.h>
#include <FW/FWFactory.h>

#include <Library/Visualizer/SingleMesh/SingleMesh.h>

namespace FW {

class MeshRender : public SingleMesh {
	public:
		typedef std::shared_ptr<MeshRender> Ptr;
		typedef std::weak_ptr<MeshRender>   WPtr;

		typedef Geometry::TriMesh Mesh;
		typedef Geometry::OpenMeshTraits<Mesh> Traits;

	public:
		class Factory;

	public:
		MeshRender(std::string id, fs::path meshFile);
		virtual ~MeshRender();

		void init();
		void render();
		void addProperties();
		void registerEvents();
};


class MeshRender::Factory : public FW::Factory {
	public:
		Factory();
		virtual ~Factory();

		void init();
		Visualizer::Ptr addVisualizer();
};


} // FW

#endif /* MESHRENDERVIS_H_ */

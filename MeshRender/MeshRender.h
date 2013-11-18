#ifndef MESHRENDERVIS_H_
#define MESHRENDERVIS_H_

#include <FW/FWVisualizer.h>
#include <FW/FWFactory.h>

#include <Library/Geometry/OpenMeshTraits.h>
#include <Library/Rendered/Mesh.h>

namespace FW {

class MeshRender : public Visualizer {
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

	protected:
		fs::path                   m_meshFile;
		std::shared_ptr<Mesh>      m_mesh;
		Rendered::Mesh<Mesh>::Ptr  m_rm;
		ShaderProgram::Ptr         m_program;
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

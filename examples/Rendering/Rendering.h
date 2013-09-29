#ifndef RENDERINGVIS_H_
#define RENDERINGVIS_H_

#include <FW/Visualizer.h>
#include <FW/Factory.h>
#include <Library/Rendered/Vectors.h>

namespace FW {


class Rendering : public Visualizer {
	public:
		class Factory;

	public:
		Rendering(std::string id);
		virtual ~Rendering();

		void init();
		void render();
		void addProperties();
		void colorize();

	protected:
		Rendered::Vectors::Ptr m_vectors;
		Eigen::Vector4f m_colX;
		Eigen::Vector4f m_colY;
		Eigen::Vector4f m_colZ;
};


class Rendering::Factory : public FW::Factory {
	public:
		Factory();
		virtual ~Factory();

		void init();
		Visualizer::Ptr addVisualizer();
};


} // FW

#endif /* RENDERINGVIS_H_ */

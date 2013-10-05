#ifndef SKELETONVIS_H_
#define SKELETONVIS_H_

#include <FW/Visualizer.h>

namespace FW {


class Skeleton : public Visualizer {
	public:
		Skeleton(std::string id);
		virtual ~Skeleton();

		void init();
		void render();
		void addProperties();
		void registerEvents();
};


} // FW

#endif /* SKELETONVIS_H_ */

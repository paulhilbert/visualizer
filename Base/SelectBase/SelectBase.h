#ifndef SELECTBASEVIS_H_
#define SELECTBASEVIS_H_

#include <FW/Visualizer.h>

namespace FW {


class SelectBase : public Visualizer {
	public:
		SelectBase(std::string id);
		virtual ~SelectBase();

		void init();
		void render();
		void addProperties();
		void registerEvents();

		virtual void areaSelectStart(int x, int y);
		virtual void areaSelectDrag(int x, int y);
		virtual void areaSelectStop(int x, int y);
};


} // FW

#endif /* SELECTBASEVIS_H_ */

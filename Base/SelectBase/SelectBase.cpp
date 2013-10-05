#include "SelectBase.h"

namespace FW {

SelectBase::SelectBase(std::string id) : Visualizer(id) {
}

SelectBase::~SelectBase() {
}

void SelectBase::init() {
	addProperties();
	registerEvents();
}

void SelectBase::render() {
}

void SelectBase::addProperties() {
}

void SelectBase::registerEvents() {
	fw()->events()->connect<void (int, int)>("LEFT_DRAG_START", [&] (int x, int y) {
		areaSelectStart(x, y);
	});
	fw()->events()->connect<void (int, int, int, int)>("LEFT_DRAG", [&] (int dx, int dy, int x, int y) {
		areaSelectDrag(x, y);
	});
	fw()->events()->connect<void (int, int)>("LEFT_DRAG_STOP", [&] (int x, int y) {
		areaSelectStop(x, y);
	});
}

void SelectBase::areaSelectStart(int x, int y) {
}

void SelectBase::areaSelectDrag(int x, int y) {
}

void SelectBase::areaSelectStop(int x, int y) {
}

} // FW


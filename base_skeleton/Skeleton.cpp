#include "Skeleton.h"

namespace FW {

Skeleton::Skeleton(std::string id) : Visualizer(id) {
}

Skeleton::~Skeleton() {
}

void Skeleton::init() {
	addProperties();
	registerEvents();
}

void Skeleton::render() {
}

void Skeleton::addProperties() {
}

void Skeleton::registerEvents() {
}

} // FW


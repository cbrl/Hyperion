#include "system_mgr.h"


SystemMgr::SystemMgr() {

	allocator = std::make_unique<LinearAllocator>(alloc_memory_size);
}


SystemMgr::~SystemMgr() {

	// Delete all of the systems
	for (auto& pair : systems) {
		pair.second->~ISystem();
		pair.second = nullptr;
	}
}


void SystemMgr::update(Engine& engine) {

	// Pre Update
	for (const auto& pair : systems) {
		if (pair.second->isActive()) {
			pair.second->preUpdate(engine);
		}
	}

	// Update
	for (const auto& pair : systems) {
		if (pair.second->isActive()) {
			pair.second->update(engine);
		}
	}

	// Post Update
	for (const auto& pair : systems) {
		if (pair.second->isActive()) {
			pair.second->postUpdate(engine);
		}
	}
}

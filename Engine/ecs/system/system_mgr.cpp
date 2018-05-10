#include "stdafx.h"
#include "system_mgr.h"


SystemMgr::SystemMgr() {

	allocator = new LinearAllocator(alloc_memory_size);
}


SystemMgr::~SystemMgr() {

	// Delete all of the systems
	for (auto& pair : systems) {
		pair.second->~ISystem();
		pair.second = nullptr;
	}

	// Delete the allocator
	allocator->Reset();
	delete allocator;
	allocator = nullptr;
}


void SystemMgr::Update(const Engine& engine) {

	// Pre Update
	for (const auto& pair : systems) {
		if (pair.second->IsActive()) {
			pair.second->PreUpdate(engine);
		}
	}

	// Update
	for (const auto& pair : systems) {
		if (pair.second->IsActive()) {
			pair.second->Update(engine);
		}
	}

	// Post Update
	for (const auto& pair : systems) {
		if (pair.second->IsActive()) {
			pair.second->PostUpdate(engine);
		}
	}
}
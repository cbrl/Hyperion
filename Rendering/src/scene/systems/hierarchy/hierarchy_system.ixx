module;

#include "memory/handle/handle.h"

export module systems.hierarchy_system;

import ecs;
import components.hierarchy;


namespace render::systems {

export class HierarchySystem final : public ecs::System {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	HierarchySystem(ecs::ECS& ecs)
		: System(ecs)
		, on_destroy_connection(ecs.getDispatcher<ecs::EntityDestroyed>().addCallback<&HierarchySystem::onEntityDestroyed>(this)) {
	}

	HierarchySystem(const HierarchySystem&) = delete;
	HierarchySystem(HierarchySystem&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~HierarchySystem() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	HierarchySystem& operator=(const HierarchySystem&) = delete;
	HierarchySystem& operator=(HierarchySystem&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override {
	}

private:

	void onEntityDestroyed(const ecs::EntityDestroyed& event) {
		auto& ecs = this->getECS();

		if (auto* hierarchy = ecs.tryGet<Hierarchy>(event.entity)) {
			hierarchy->forEachChild(ecs, [&ecs](handle64 child) {
				ecs.destroy(child);
			});
		}
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ecs::UniqueDispatcherConnection on_destroy_connection;
};

} //namespace render::systems

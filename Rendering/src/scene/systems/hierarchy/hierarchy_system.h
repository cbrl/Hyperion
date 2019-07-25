#pragma once

#include "system/system.h"
#include "event/event_dispatcher.h"

namespace ecs {
	class ECS;
	struct EntityDestroyed;
}

namespace render::systems {

class HierarchySystem final : public ecs::System<HierarchySystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	HierarchySystem(ecs::ECS& ecs);
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
	void update() override;

private:

	void onEntityDestroyed(const ecs::EntityDestroyed& event);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ecs::UniqueDispatcherConnection on_destroy_connection;
};

} //namespace render::systems
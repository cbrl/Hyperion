#pragma once

#include "system/system.h"
#include "event/event_dispatcher.h"
#include "scene/components/hierarchy/hierarchy.h"
#include "scene/events/core_events.h"

class Transform;

namespace ecs {
	class ECS;
}

namespace render::systems {

class TransformSystem final : public ecs::System<TransformSystem> {

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TransformSystem(ecs::ECS& ecs);
	TransformSystem(const TransformSystem&) = delete;
	TransformSystem(TransformSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~TransformSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TransformSystem& operator=(const TransformSystem&) = delete;
	TransformSystem& operator=(TransformSystem&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	void onParentChanged(const Hierarchy::ParentChangedEvent& event);

	bool updateWorld(Transform& transform);

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ecs::UniqueDispatcherConnection parent_changed_connection;
};

} //namespace render::systems
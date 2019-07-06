#pragma once

#include "ecs.h"
#include "scene/components/hierarchy/hierarchy.h"
#include "scene/events/core_events.h"

class Transform;

namespace render::systems {

class TransformSystem final : public ecs::System<TransformSystem>, public ecs::EventListener {

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TransformSystem() = default;
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

	void update() override;

private:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void registerCallbacks() override;
	void onParentChanged(const Hierarchy::ParentChangedEvent& event);

	bool updateWorld(Transform& transform);
};

} //namespace render::systems
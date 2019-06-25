#pragma once

#include "ecs.h"
#include "scene/components/transform/transform.h"
#include "scene/events/core_events.h"


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
	void onParentChanged(const ecs::Entity::ParentChangedEvent& event);

	void updateWorld(Transform& transform);
};

} //namespace render::systems
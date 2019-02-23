#pragma once

#include "ecs.h"
#include "scene/components/transform/transform.h"
#include "scene/events/core_events.h"


class TransformSystem final : public System<TransformSystem>, public EventListener, public EventSender {
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

private:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void registerCallbacks() override;

	void updateWorld(Transform& transform);
	
	void onTransformNeedsUpdate(const TransformNeedsUpdate& event);
	void onParentChanged(const ParentChanged& event);
};

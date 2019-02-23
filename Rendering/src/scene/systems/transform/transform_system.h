#pragma once

#include "ecs.h"
#include "scene/components/transform/transform.h"
#include "scene/events/core_events.h"


class TransformSystem final : public System<TransformSystem>, public EventListener, public EventSender {
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void registerCallbacks() override final;

private:
	void updateWorld(Transform& transform);
	
	void onTransformNeedsUpdate(const TransformNeedsUpdate& event);
	void onParentChanged(const ParentChanged& event);
};

#pragma once

#include "ecs.h"
#include "scene/components/transform/transform.h"


class TransformSystem final : public System<TransformSystem>, public EventListener {
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void registerCallbacks() override final;
	void onTransformUpdate(const TransformEvent* event);
	void onParentChanged(const ParentChanged* event);

private:
	static void updateWorld(Transform& transform);
};

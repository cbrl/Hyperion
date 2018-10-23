#pragma once

#include "ecs.h"
#include "scene/components/transform/transform.h"


class TransformSystem final : public System<TransformSystem> {
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void update(Engine& engine) override;
	void postUpdate(Engine& engine) override;


private:
	static void updateWorld(Transform& transform);
	static XMMATRIX calculateWorld(Transform& transform);
};

#pragma once

#include "ecs.h"
#include "components/transform/transform.h"


class TransformSystem final : public System<TransformSystem> {
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void update(Engine& engine) override;
	void postUpdate(Engine& engine) override;


private:
	static void updateWorld(ECS& ecs_engine, Transform& transform);
	static XMMATRIX calculateWorld(Transform& transform);
};

#pragma once

#include "ecs.h"
#include "components/transform/transform.h"


class TransformSystem final : public System<TransformSystem> {
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void update(const Engine& engine) override;
	void postUpdate(const Engine& engine) override;


private:
	static void updateWorld(ECS& ecs_engine, Transform& transform);
	static XMMATRIX calculateWorld(Transform& transform);
};

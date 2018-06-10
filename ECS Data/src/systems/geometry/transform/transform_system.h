#pragma once

#include "ecs.h"
#include "components/geometry/transform/transform.h"
#include "components/geometry/transform/camera_transform.h"


class TransformSystem final : public System<TransformSystem> {
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void update(const Engine& engine) override;
	void postUpdate(const Engine& engine) override;


private:
	template<typename TransformT>
	void updateWorld(ECS& ecs_engine, TransformT& transform);

	static XMMATRIX calculateWorld(Transform& transform);
	static XMMATRIX calculateWorld(CameraTransform& transform);
};


#include "transform_system.tpp"

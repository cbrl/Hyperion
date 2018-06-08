#pragma once

#include "ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"


class TransformSystem final : public System<TransformSystem> {
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

		void update(const Engine& engine) override;
		void postUpdate(const Engine& engine) override;


	private:
		template<typename TransformT>
		void UpdateWorld(ECS& ecs_engine, TransformT& transform);

		const XMMATRIX CalculateWorld(Transform& transform) const;
		const XMMATRIX CalculateWorld(CameraTransform& transform) const;
};


#include "transform_system.tpp"
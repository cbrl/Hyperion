#pragma once

#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"


class TransformSystem final : public System<TransformSystem> {
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

		void Update(const Engine& engine) override;
		void PostUpdate(const Engine& engine) override {
			ECS::Get()->ForEach<Transform>([&](Transform& transform) {
				transform.updated = false;
			});
			ECS::Get()->ForEach<CameraTransform>([&](CameraTransform& transform) {
				transform.updated = false;
			});
		}


	private:
		template<typename TransformT>
		void UpdateWorld(TransformT& transform);

		XMMATRIX CalculateWorld(Transform& transform) const;
		XMMATRIX CalculateWorld(CameraTransform& transform) const;
};


#include "transform_system.tpp"
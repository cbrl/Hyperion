#pragma once

#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"


class TransformSystem final : public System<TransformSystem> {
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

		void Update(const Engine& engine) override;
		void PostUpdate(const Engine& engine) override;


	private:
		template<typename TransformT>
		void UpdateWorld(TransformT& transform);

		XMMATRIX CalculateWorld(Transform& transform) const;
		XMMATRIX CalculateWorld(CameraTransform& transform) const;
};


#include "transform_system.tpp"
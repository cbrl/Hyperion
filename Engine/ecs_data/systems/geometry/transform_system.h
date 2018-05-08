#pragma once

#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"


class TransformSystem final : public System<TransformSystem> {
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

		void Update(float dt);
		void PostUpdate(float dt);


	private:
		template<typename TransformT>
		void UpdateWorld(TransformT& transform) {

			XMMATRIX world = XMMatrixIdentity();

			// Calculate the new object-to-world matrix
			world *= CalculateWorld(transform);


			// If the transform is the child of another, then multiply
			// the new object-to-world matrix by the p arent's matrix.
			if (transform.GetParent() == Handle64::invalid_handle) {
				if (!transform.needs_update) return;
			}
			else {
				auto parent     = ECS::Get()->GetComponent<Transform>(transform.GetParent());
				auto parent_cam = ECS::Get()->GetComponent<CameraTransform>(transform.GetParent());

				if (parent) {
					UpdateWorld(*parent);
					world *= parent->GetWorld();

					if (parent->needs_update) {
						transform.needs_update = true;
					}
				}
				else if (parent_cam) {
					UpdateWorld(*parent_cam);
					world *= parent_cam->GetWorld();

					if (parent_cam->needs_update) {
						transform.needs_update = true;
					}
				}
			}


			// Set the new matrix
			transform.world = world;
		}

		XMMATRIX CalculateWorld(Transform& transform) const;
		XMMATRIX CalculateWorld(CameraTransform& transform) const;
};
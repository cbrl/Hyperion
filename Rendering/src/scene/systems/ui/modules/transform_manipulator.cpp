#include "transform_manipulator.h"
#include "engine/engine.h"

TransformManipulator::TransformManipulator(Input& input, KeyConfig& key_config)
	: input(input)
	, key_config(key_config) {
	key_config.tryBindKey("ObjRotate",    Keyboard::R);
	key_config.tryBindKey("ObjTranslate", Keyboard::T);
	key_config.tryBindKey("ObjScale",     Keyboard::Y);
}

void TransformManipulator::draw(Engine& engine, handle64 selected_entity) {
	auto& ecs = engine.getScene().getECS();

	if (not ecs.valid(selected_entity)) {
		return;
	}

	// Draw transform manipulation tool. Only for the primary camera since it
	// doesn't work well when drawing multiple transform tools at once.
	if (auto* transform = ecs.tryGet<Transform>(selected_entity)) {
		if (not transform->isActive())
			return;

		bool first = true;
		ecs.forEach<PerspectiveCamera, Transform>([&](handle64 entity) {
			if (first) {
				auto& camera          = ecs.get<PerspectiveCamera>(entity);
				auto& camera_tranform = ecs.get<Transform>(entity);

				drawTransformManipulator(ecs, *transform, camera, camera_tranform);
				first = false;
			}
		});
		ecs.forEach<OrthographicCamera, Transform>([&](handle64 entity) {
			if (first) {
				auto& camera          = ecs.get<PerspectiveCamera>(entity);
				auto& camera_tranform = ecs.get<Transform>(entity);

				drawTransformManipulator(ecs, *transform, camera, camera_tranform);
				first = false;
			}
		});
	}
}


template<typename CameraT>
void TransformManipulator::drawTransformManipulator(ecs::ECS& ecs, Transform& transform, const CameraT& camera, const Transform& camera_transform) {
	// Set orthographic mode for ortho camera
	if constexpr (std::is_same_v<CameraT, OrthographicCamera>) {
		ImGuizmo::SetOrthographic(true);
	}
	else {
		ImGuizmo::SetOrthographic(false);
	}

	//----------------------------------------------------------------------------------
	// Matrices
	//----------------------------------------------------------------------------------
	const XMMATRIX world_to_camera      = camera_transform.getWorldToObjectMatrix();
	const XMMATRIX camera_to_projection = camera.getCameraToProjectionMatrix();

	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&view, world_to_camera);
	XMStoreFloat4x4(&projection, camera_to_projection);

	const XMMATRIX transform_matrix = transform.getObjectToWorldMatrix();
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, transform_matrix);

	//----------------------------------------------------------------------------------
	// View Rect
	//----------------------------------------------------------------------------------
	const u32_2 top_left = camera.getViewport().getTopLeft();
	const u32_2 size     = camera.getViewport().getSize();
	ImGuizmo::SetRect((float)top_left[0], (float)top_left[1], (float)size[0], (float)size[1]);

	//----------------------------------------------------------------------------------
	// Operation & Mode
	//----------------------------------------------------------------------------------
	if (input.get().isKeyPressed(key_config.get().getKey("ObjRotate"))) {
		operation = ImGuizmo::ROTATE;
	}
	if (input.get().isKeyPressed(key_config.get().getKey("ObjTranslate"))) {
		operation = ImGuizmo::TRANSLATE;
	}
	if (input.get().isKeyPressed(key_config.get().getKey("ObjScale"))) {
		operation = ImGuizmo::SCALE;
	}

	//----------------------------------------------------------------------------------
	// Manipulate
	//----------------------------------------------------------------------------------
	ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], operation, mode, &matrix.m[0][0]);

	if (ImGuizmo::IsUsing()) {
		f32_3 translation;
		f32_3 rotation;
		f32_3 scale;

		if (auto* hierarchy = ecs.tryGet<Hierarchy>(transform.getOwner());
		    hierarchy && ecs.has<Transform>(hierarchy->getParent())) {
			// If the transform is a child of another, then the matrix needs to be multiplied by
			// the inverse of the parent's matrix to obtain the local transformation.
			auto& parent_transform = ecs.get<Transform>(hierarchy->getParent());

			const XMMATRIX world_to_parent = parent_transform.getWorldToObjectMatrix();
			const XMMATRIX object_to_parent = XMLoadFloat4x4(&matrix) * world_to_parent;
			XMFLOAT4X4 new_matrix;
			XMStoreFloat4x4(&new_matrix, object_to_parent);

			ImGuizmo::DecomposeMatrixToComponents(&new_matrix.m[0][0], translation.data(), rotation.data(), scale.data());
		}
		else {
			ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], translation.data(), rotation.data(), scale.data());
		}

		// ImGuizmo outputs rotation in degrees
		rotation = {XMConvertToRadians(rotation[0]), XMConvertToRadians(rotation[1]), XMConvertToRadians(rotation[2])};

		transform.setPosition(translation);
		transform.setRotation(rotation);
		transform.setScale(scale);
	}
}

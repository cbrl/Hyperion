#include "transform_manipulator.h"
#include "engine/engine.h"

TransformManipulator::TransformManipulator(Input& input, KeyConfig& key_config)
	: input(input)
	, key_config(key_config) {
	key_config.bindIfNotBound("ObjRotate",    Keyboard::R);
	key_config.bindIfNotBound("ObjTranslate", Keyboard::T);
	key_config.bindIfNotBound("ObjScale",     Keyboard::Y);
}

void TransformManipulator::draw(Engine& engine, ecs::EntityPtr selected_entity) {
	if (!selected_entity) {
		return;
	}

	auto& scene = engine.getScene();

	// Draw transform manipulation tool. Only for the primary camera since it
	// doesn't work well when drawing multiple transform tools at once.
	if (auto* transform = selected_entity->getComponent<Transform>()) {
		bool first = true;
		scene.forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
			if (first) {
				drawTransformManipulator(*transform, camera);
				first = false;
			}
		});
		scene.forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
			if (first) {
				drawTransformManipulator(*transform, camera);
				first = false;
			}
		});
	}
}


template<typename CameraT>
void TransformManipulator::drawTransformManipulator(Transform& transform, CameraT& camera) {

	// Active check
	if (!transform.isActive())
		return;

	// Get camera transform
	auto* camera_transform = camera.getOwner()->getComponent<Transform>();
	if (!camera_transform)
		return;

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
	XMMATRIX world_to_camera = camera_transform->getWorldToObjectMatrix();
	XMMATRIX camera_to_projection = camera.getCameraToProjectionMatrix();

	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&view, world_to_camera);
	XMStoreFloat4x4(&projection, camera_to_projection);

	XMMATRIX transform_matrix = transform.getObjectToWorldMatrix();
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, transform_matrix);

	//----------------------------------------------------------------------------------
	// View Rect
	//----------------------------------------------------------------------------------
	auto top_left = camera.getViewport().getTopLeft();
	auto size = camera.getViewport().getSize();
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

	// Allowing the transform matrix to be set here would eliminate the need to decompose the matrix
	if (ImGuizmo::IsUsing()) {
		f32_3 translation;
		f32_3 rotation;
		f32_3 scale;

		if (!transform.getOwner()->hasParent()) {
			ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], translation.data(), rotation.data(), scale.data());
		}
		else {
			// If the transform is a child of another, then the matrix needs to be multiplied by the inverse of
			// the parent's matrix to obtain the local transformation.
			auto* parent_transform = transform.getOwner()->getParent()->getComponent<Transform>();

			XMMATRIX world_to_parent = parent_transform->getWorldToObjectMatrix();
			XMMATRIX relative_transform = XMLoadFloat4x4(&matrix) * world_to_parent;
			XMFLOAT4X4 new_matrix;
			XMStoreFloat4x4(&new_matrix, relative_transform);

			ImGuizmo::DecomposeMatrixToComponents(&new_matrix.m[0][0], translation.data(), rotation.data(), scale.data());
		}

		// ImGuizmo outputs rotation in degrees
		rotation = {XMConvertToRadians(rotation[0]), XMConvertToRadians(rotation[1]), XMConvertToRadians(rotation[2])};

		transform.setPosition(translation);
		transform.setRotation(rotation);
		transform.setScale(scale);
	}
}

#include "user_interface.h"
#include "system_menu.h"
#include "scene_tree.h"
#include "entity_details_window.h"
#include "new_model_menu.h"
#include "selectable_tree.h"
#include "text_edit_window.h"
#include "metrics_window.h"

#include "log/log.h"
#include "os/windows/win_utils.h"

#include "engine/engine.h"
#include "resource/resource_mgr.h"
#include "scene/scene.h"

#include "entities/entities.h"
#include "components/components.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_addons/ImGuizmo/ImGuizmo.h"
#include "imgui_addons/metrics_gui/metrics_gui/metrics_gui.h"
#include "imgui_addons/ImGuiColorTextEdit/TextEditor.h"


template<typename CameraT>
void DrawTransformManipulator(Transform& transform, CameraT& camera, Input& input) {

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
	static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	static ImGuizmo::MODE mode = ImGuizmo::WORLD;

	if (input.isKeyPressed(Keyboard::R)) {
		operation = ImGuizmo::ROTATE;
	}
	if (input.isKeyPressed(Keyboard::T)) {
		operation = ImGuizmo::TRANSLATE;
	}
	if (input.isKeyPressed(Keyboard::Y)) {
		operation = ImGuizmo::SCALE;
	}

	//----------------------------------------------------------------------------------
	// Manipulate
	//----------------------------------------------------------------------------------
	ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], operation, mode, &matrix.m[0][0]);

	// Allowing the transform matrix to be set here would eliminate the need to decompose the matrix
	if (ImGuizmo::IsUsing()) {
		vec3_f32 translation;
		vec3_f32 rotation;
		vec3_f32 scale;

		if (!transform.getOwner()->hasParent()) {
			ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], translation.data(), rotation.data(), scale.data());
		}
		else {
			// If the transform is a child of another, then the matrix needs to be multiplied by the inverse of
			// the parent's matrix to obtain the local transformation.
			auto* parent_transform = transform.getOwner()->getParent()->getComponent<Transform>();

			XMMATRIX world_to_parent    = parent_transform->getWorldToObjectMatrix();
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




void UserInterface::update(Engine& engine) {

	//ImGui::ShowDemoWindow();
	ImGuizmo::BeginFrame();

	auto& device       = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto& scene        = engine.getScene();

	// Draw the system menu
	static SystemMenu system_menu(engine);
	static bool system_menu_open = true;
	if (engine.getInput().isKeyPressed(Keyboard::F3)) {
		system_menu_open = !system_menu_open;
	}
	if (system_menu_open) {
		system_menu.draw(engine);
	}

	// Draw the system metrics
	static MetricsWindow metrics(engine);
	metrics.draw(engine);

	// Draw the shader editor
	static TextEditWindow text_editor;
	text_editor.draw(engine, system_menu.getTextEditorState());

	// Draw the scene tree
	static SceneTree scene_tree;
	scene_tree.draw(scene);

	// Draw the selected entity's details
	static EntityDetailsWindow entity_details;
	entity_details.draw(engine, scene_tree.getSelectedEntity());

	// Draw transform manipulation tool. Only for the primary camera since it
	// doesn't work well when drawing multiple transform tools at once.
	auto entity_ptr = scene_tree.getSelectedEntity();
	if (entity_ptr) {
		if (auto* transform = entity_ptr->getComponent<Transform>()) {
			bool first = true;
			scene.forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
				if (first) {
					DrawTransformManipulator(*transform, camera, engine.getInput());
					first = false;
				}
			});
			scene.forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
				if (first) {
					DrawTransformManipulator(*transform, camera, engine.getInput());
					first = false;
				}
			});
		}
	}

	ImGui::End();
}
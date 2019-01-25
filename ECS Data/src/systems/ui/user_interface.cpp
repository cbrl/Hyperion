#include "user_interface.h"
#include "new_model_menu.h"
#include "selectable_tree.h"

#include "engine/engine.h"

#include "entities/entities.h"
#include "components/components.h"

#include "resource/resource_mgr.h"
#include "scene/scene.h"
#include "log/log.h"
#include "os/windows/win_utils.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_addons/ImGuizmo/ImGuizmo.h"
#include "imgui_addons/metrics_gui/metrics_gui/metrics_gui.h"


//----------------------------------------------------------------------------------
// Scene Tree global
//----------------------------------------------------------------------------------
SelectableTree<EntityPtr> g_scene_tree;



//----------------------------------------------------------------------------------
// ImGuiTreeNodeFlags Helpers
//----------------------------------------------------------------------------------
ImGuiTreeNodeFlags MakeTreeNodeFlags() {
	return ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
}

ImGuiTreeNodeFlags MakeTreeLeafFlags() {
	return ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
}

ImGuiTreeNodeFlags MakeTreeNodeFlags(bool selected) {
	return ImGuiTreeNodeFlags_OpenOnArrow
	       | ImGuiTreeNodeFlags_OpenOnDoubleClick
	       | (selected ? ImGuiTreeNodeFlags_Selected : 0);
}

ImGuiTreeNodeFlags MakeTreeLeafFlags(bool selected) {
	return ImGuiTreeNodeFlags_Leaf
	       | ImGuiTreeNodeFlags_Bullet
	       | (selected ? ImGuiTreeNodeFlags_Selected : 0);
}



//----------------------------------------------------------------------------------
//
//   Object Details
//
//----------------------------------------------------------------------------------

template<typename T>
void DrawComponentState(T& component, gsl::czstring<> name = nullptr) {
	if (name) {
		ImGui::InputText("", const_cast<char*>(name), std::strlen(name), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
	}
	bool state = component.isActive();
	if (ImGui::Checkbox("Active", &state))
		component.setActive(state);
	ImGui::Spacing();
}


void DrawDetails(Transform& transform) {

	DrawComponentState(transform);

	vec3_f32 position;
	vec3_f32 rotation;
	vec3_f32 scale;
	XMStore(&position, transform.getPosition());
	XMStore(&rotation, transform.getRotation());
	XMStore(&scale, transform.getScale());

	if (ImGui::DragFloat3("Position", position.data(), 0.05f, -FLT_MAX, FLT_MAX))
		transform.setPosition(position);

	if (ImGui::DragFloat3("Rotation", rotation.data(), 0.05f, -FLT_MAX, FLT_MAX))
		transform.setRotation(rotation);

	if (ImGui::DragFloat3("Scale", scale.data(), 0.05f, -FLT_MAX, FLT_MAX))
		transform.setScale(scale);
}


void DrawCameraSettings(CameraSettings& settings) {

	ImGui::Spacing();
	ImGui::Text("Render Settings");
	ImGui::Separator();

	//----------------------------------------------------------------------------------
	// Render Mode
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring<> render_mode_names[] = {
	    "Forward",
	    "Forward+",
	    "Deferred"};
	static constexpr RenderMode render_modes[] = {
	    RenderMode::Forward,
	    RenderMode::ForwardPlus,
	    RenderMode::Deferred};

	auto render_mode = static_cast<int>(settings.getRenderMode());
	if (ImGui::Combo("Render Mode", &render_mode, render_mode_names, static_cast<int>(std::size(render_mode_names))))
		settings.setRenderMode(static_cast<RenderMode>(render_mode));


	//----------------------------------------------------------------------------------
	// Lighting Mode
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring<> light_mode_names[] = {
	    "Default",
	    "FalseColorFullbright",
	    "FalseColorNormal",
	    "FalseColorDepth"};
	static constexpr LightingMode light_modes[] = {
	    LightingMode::Default,
	    LightingMode::FalseColorFullbright,
	    LightingMode::FalseColorNormal,
	    LightingMode::FalseColorDepth};

	auto light_mode = static_cast<int>(settings.getLightingMode());
	if (ImGui::Combo("Lighting Mode", &light_mode, light_mode_names, static_cast<int>(std::size(light_mode_names))))
		settings.setLightingMode(static_cast<LightingMode>(light_mode));


	//----------------------------------------------------------------------------------
	// BRDF
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring<> brdf_names[] = {
	    "Lambert",
	    "Blinn-Phong",
	    "Cook-Torrance"};
	static constexpr BRDF brdfs[] = {
	    BRDF::Lambert,
	    BRDF::BlinnPhong,
	    BRDF::CookTorrance};

	auto brdf = static_cast<int>(settings.getBRDF());
	if (ImGui::Combo("BRDF", &brdf, brdf_names, static_cast<int>(std::size(brdf_names))))
		settings.setBRDF(static_cast<BRDF>(brdf));


	//----------------------------------------------------------------------------------
	// Render Options
	//----------------------------------------------------------------------------------
	ImGui::Spacing();

	auto bounding_volumes = settings.hasRenderOption(RenderOptions::BoundingVolume);
	if (ImGui::Checkbox("Bounding Volumes", &bounding_volumes)) {
		settings.toggleRenderOption(RenderOptions::BoundingVolume);
	}
	if (bounding_volumes) {
		auto color = settings.getBoundingVolumeColor();
		if (ImGui::ColorEdit3("BV Color", color.data())) {
			settings.setBoundingVolumeColor(color);
		}
	}

	auto wireframe = settings.hasRenderOption(RenderOptions::Wireframe);
	if (ImGui::Checkbox("Wireframe", &wireframe)) {
		settings.toggleRenderOption(RenderOptions::Wireframe);
	}
	if (wireframe) {
		auto color = settings.getWireframeColor();
		if (ImGui::ColorEdit3("Wire Color", color.data())) {
			settings.setWireframeColor(color);
		}
	}


	//----------------------------------------------------------------------------------
	// Fog
	//----------------------------------------------------------------------------------
	ImGui::Spacing();
	ImGui::Text("Fog");
	ImGui::Separator();

	auto& fog = settings.getFog();
	ImGui::DragFloat("Fog Density", &fog.density, 0.001f, 0.0f, FLT_MAX);
	ImGui::ColorEdit3("Fog Color", fog.color.data());
}


void DrawDetails(PerspectiveCamera& camera) {

	DrawComponentState(camera);

	//----------------------------------------------------------------------------------
	// FOV
	//----------------------------------------------------------------------------------
	f32 fov = camera.getFOV();
	if (ImGui::DragFloat("FOV", &fov, 0.01f, (XM_PI / 8.0f), (XM_2PI / 3.0f))) {
		camera.setFOV(fov);
	}

	//----------------------------------------------------------------------------------
	// Viewport
	//----------------------------------------------------------------------------------
	auto& vp = camera.getViewport();
	vec2_u32 size = vp.getSize();
	vec2_u32 pos = vp.getTopLeft();

	static constexpr u32 v_min_size = 1;
	static constexpr u32 v_min_pos = 0;
	static constexpr u32 v_max = 15360;

	if (ImGui::DragScalarN("Viewport Size", ImGuiDataType_U32, size.data(), 2, 1.0f, &v_min_size, &v_max)) {
		vp.setSize(size);
	}
	if (ImGui::DragScalarN("Viewport Top-Left", ImGuiDataType_U32, pos.data(), 2, 1.0f, &v_min_pos, &v_max)) {
		vp.setTopLeft(pos);
	}

	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------
	vec2_f32 depth = camera.getZDepth();
	if (ImGui::DragFloat2("Depth Range", depth.data(), 1.0f, 0.01f, FLT_MAX)) {
		camera.setZDepth(depth);
	}


	DrawCameraSettings(camera.getSettings());
}


void DrawDetails(OrthographicCamera& camera) {

	DrawComponentState(camera);

	//----------------------------------------------------------------------------------
	// Ortho Size
	//----------------------------------------------------------------------------------
	auto ortho_size = camera.getSize();
	if (ImGui::DragFloat2("Orthographic Size", ortho_size.data(), 0.01f, 1.0f, 100.0f)) {
		camera.setSize(ortho_size);
	}

	//----------------------------------------------------------------------------------
	// Viewport
	//----------------------------------------------------------------------------------
	auto& vp = camera.getViewport();
	vec2_u32 size = vp.getSize();
	vec2_u32 pos = vp.getTopLeft();

	static constexpr u32 v_min_size = 1;
	static constexpr u32 v_min_pos = 0;
	static constexpr u32 v_max = 15360;

	if (ImGui::DragScalarN("Viewport Size", ImGuiDataType_U32, size.data(), 2, 1.0f, &v_min_size, &v_max)) {
		vp.setSize(size);
	}
	if (ImGui::DragScalarN("Viewport Top-Left", ImGuiDataType_U32, pos.data(), 2, 1.0f, &v_min_pos, &v_max)) {
		vp.setTopLeft(pos);
	}

	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------
	vec2_f32 depth = camera.getZDepth();
	if (ImGui::DragFloat2("Depth Range", depth.data(), 1.0f, 0.01f, FLT_MAX))
		camera.setZDepth(depth);


	DrawCameraSettings(camera.getSettings());
}


void DrawDetails(CameraMovement& movement) {

	DrawComponentState(movement);

	f32 max_velocity = movement.getMaxVelocity();
	f32 acceleration = movement.getAcceleration();
	f32 deceleration = movement.getDeceleration();

	if (ImGui::InputFloat("Max Velocity", &max_velocity))
		movement.setMaxVelocity(max_velocity);

	if (ImGui::InputFloat("Acceleration", &acceleration))
		movement.setAcceleration(acceleration);

	if (ImGui::InputFloat("Deceleration", &deceleration))
		movement.setDeceleration(deceleration);
}


void DrawDetails(MouseRotation& rotation) {

	DrawComponentState(rotation);

	f32 sensitivity = rotation.getSensitivity();

	if (ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f, 0.0f, FLT_MAX)) {
		rotation.setSensitivity(sensitivity);
	}
}


void DrawDetails(Text& text) {

	DrawComponentState(text);

	std::string str = WstrToStr(text.getText());

	if (ImGui::InputTextMultiline("Text", &str)) {
		text.setText(StrToWstr(str));
	}
}


void DrawDetails(Model& model) {

	ImGui::BeginGroup();

	DrawComponentState(model);

	ImGui::Separator();

	// Material properties
	auto& mat = model.getMaterial();
	ImGui::Text("Material: %s", mat.name.c_str());

	ImGui::ColorEdit4("Base Color", mat.params.base_color.data());
	ImGui::DragFloat("Metalness", &mat.params.metalness, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Roughness", &mat.params.roughness, 0.01f, 0.0f, 1.0f);
	ImGui::ColorEdit3("Emissive", mat.params.emissive.data());

	ImGui::Separator();

	bool shadows = model.castsShadows();
	if (ImGui::Checkbox("Casts Shadows", &shadows))
		model.setShadows(shadows);

	ImGui::EndGroup();
}

void DrawDetails(ModelNode& node) {
	if (ImGui::Button("Active"))
		node.setModelsActive(true);
	ImGui::SameLine();
	if (ImGui::Button("Inactive"))
		node.setModelsActive(false);

	ImGui::Spacing();
}

// Draw the model hierarchy
void DrawModelNodes(ModelNode& node) {

	if (ImGui::TreeNodeEx(&node, MakeTreeNodeFlags(), "Node: %s", node.getName().c_str())) {

		DrawDetails(node);

		node.forEachModel([](Model& model) {
			if (ImGui::TreeNodeEx(&model, MakeTreeNodeFlags(), "Model: %s", model.getName().c_str())) {
				DrawDetails(model);
				ImGui::TreePop();
			}
		});
		node.forEachNode([](ModelNode& node) {
			DrawModelNodes(node);
		});

		ImGui::TreePop();
	}
}

void DrawDetails(ModelRoot& root) {
	DrawComponentState(root);
	DrawModelNodes(root.getRootNode());
}


void DrawDetails(AmbientLight& light) {

	DrawComponentState(light);

	auto color = light.getColor();
	if (ImGui::ColorEdit3("Ambient Color", color.data()))
		light.setColor(color);
}


void DrawDetails(DirectionalLight& light) {

	DrawComponentState(light);

	// Base Color
	ImGui::ColorEdit3("Base Color", light.getBaseColor().data());

	// Intensity
	auto intensity = light.getIntensity();
	if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, FLT_MAX))
		light.setIntensity(intensity);

	// Range
	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.1f, FLT_MAX))
		light.setRange(range);

	// Size
	auto proj_size = light.getSize();
	if (ImGui::DragFloat2("Size", proj_size.data(), 0.01f, 0.01f, FLT_MAX))
		light.setSize(proj_size);

	// Shadows
	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(PointLight& light) {

	DrawComponentState(light);

	// Base Color
	ImGui::ColorEdit3("Base Color", light.getBaseColor().data());

	// Intensity
	auto intensity = light.getIntensity();
	if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, FLT_MAX))
		light.setIntensity(intensity);

	// Range
	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, FLT_MAX))
		light.setRange(range);

	// Attenuation
	ImGui::DragFloat3("Attenuation", light.getAttenuation().data(), 0.01f, 0.0f, 1.0f);

	// Shadows
	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(SpotLight& light) {

	DrawComponentState(light);

	// Base Color
	ImGui::ColorEdit3("Base Color", light.getBaseColor().data());

	// Intensity
	auto intensity = light.getIntensity();
	if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, FLT_MAX))
		light.setIntensity(intensity);

	// Range
	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, FLT_MAX))
		light.setRange(range);

	// Attenuation
	ImGui::DragFloat3("Attenuation", light.getAttenuation().data(), 0.01f, 0.0f, 1.0f);

	// Umbra Angle
	auto umbra = light.getUmbraAngle();
	if (ImGui::DragFloat("Umbra", &umbra, 0.01f, 0.01f, XM_PIDIV2))
		light.setUmbraAngle(umbra);

	// Penumbra Angle
	auto penumbra = light.getPenumbraAngle();
	if (ImGui::DragFloat("Penumbra", &penumbra, 0.01f, 0.01f, XM_PIDIV2))
		light.setPenumbraAngle(penumbra);

	// Shadows
	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(AxisRotation& rotation) {

	DrawComponentState(rotation);

	//----------------------------------------------------------------------------------
	// X Rotation
	//----------------------------------------------------------------------------------
	bool x_enable = rotation.hasAxis(AxisRotation::Axis::X);

	if (ImGui::Checkbox("X Axis", &x_enable)) {
		if (x_enable)
			rotation.addAxis(AxisRotation::Axis::X);
		else
			rotation.removeAxis(AxisRotation::Axis::X);
	}
	if (x_enable) {
		f32 speed_x = rotation.getSpeedX();
		if (ImGui::DragFloat("X Speed", &speed_x, 0.05f, -FLT_MAX, FLT_MAX)) {
			rotation.setSpeedX(speed_x);
		}
	}

	//----------------------------------------------------------------------------------
	// Y Rotation
	//----------------------------------------------------------------------------------
	bool y_enable = rotation.hasAxis(AxisRotation::Axis::Y);

	if (ImGui::Checkbox("Y Axis", &y_enable)) {
		if (y_enable)
			rotation.addAxis(AxisRotation::Axis::Y);
		else
			rotation.removeAxis(AxisRotation::Axis::Y);
	}
	if (y_enable) {
		f32 speed_y = rotation.getSpeedY();
		if (ImGui::DragFloat("Y Speed", &speed_y, 0.05f, -FLT_MAX, FLT_MAX)) {
			rotation.setSpeedY(speed_y);
		}
	}

	//----------------------------------------------------------------------------------
	// Z Rotation
	//----------------------------------------------------------------------------------
	bool z_enable = rotation.hasAxis(AxisRotation::Axis::Z);

	if (ImGui::Checkbox("Z Axis", &z_enable)) {
		if (z_enable)
			rotation.addAxis(AxisRotation::Axis::Z);
		else
			rotation.removeAxis(AxisRotation::Axis::Z);
	}
	if (z_enable) {
		f32 speed_z = rotation.getSpeedZ();
		if (ImGui::DragFloat("Z Speed", &speed_z, 0.05f, -FLT_MAX, FLT_MAX)) {
			rotation.setSpeedZ(speed_z);
		}
	}
}


void DrawDetails(AxisOrbit& orbit) {

	DrawComponentState(orbit);

	auto axis = XMStore<vec3_f32>(orbit.getAxis());
	if (ImGui::InputFloat3("Axis", axis.data())) {
		orbit.setAxis(axis);
	}
}


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
		} else {
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




//----------------------------------------------------------------------------------
//
//   Entity Details
//
//----------------------------------------------------------------------------------

// Draw a node in the tree, and its details if selected
template<typename T>
void DrawComponentNode(gsl::czstring<> text, T& item) {

	static IComponent* selected = nullptr;

	bool dont_delete = true;
	ImGui::PushID(&item);
	if (ImGui::CollapsingHeader(text, &dont_delete)) {
		selected = &item;
		DrawDetails(item);
	}
	ImGui::PopID();
	if (!dont_delete && selected) {
		selected->getOwner()->removeComponent(selected);
	}
}


// Draw the menu in the entity details window
void DrawAddComponentMenu(ID3D11Device& device, ResourceMgr& resource_mgr) {

	if (ImGui::BeginMenu("Add Component")) {

		if (ImGui::MenuItem("Orthographic Camera")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<OrthographicCamera>(device, vec2_u32{480, 480});
		}
		if (ImGui::MenuItem("Perspective Camera")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<PerspectiveCamera>(device, vec2_u32{480, 480});
		}
		if (ImGui::MenuItem("Directional Light")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<DirectionalLight>();
		}
		if (ImGui::MenuItem("Point Light")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<PointLight>();
		}
		if (ImGui::MenuItem("Spot Light")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<SpotLight>();
		}

		if (ImGui::BeginMenu("Model")) {
			if (ImGui::MenuItem("From file")) {
				if (auto file = OpenFilePicker(); fs::exists(file)) {
					ModelConfig<VertexPositionNormalTexture> config;
					auto bp = resource_mgr.getOrCreate<ModelBlueprint>(file, config);
					if (auto entity = g_scene_tree.getSelected())
						entity->addComponent<ModelRoot>(device, bp);
				} else
					Logger::log(LogLevel::err, "Failed to open file dialog");
			}

			if (ImGui::BeginMenu("Geometric Shape")) {
				NewModelMenu::DoMenu();
				ImGui::EndMenu();
			}

			ImGui::EndMenu(); //Model
		}

		ImGui::EndMenu(); //Add Component
	}
}


// Draw the details panel for the specified entity
void DrawEntityDetails(Entity& entity, Engine& engine) {

	auto& device = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto& scene = engine.getScene();
	const auto& entities = scene.getEntities();

	//----------------------------------------------------------------------------------
	// Begin Window
	//----------------------------------------------------------------------------------
	if (!ImGui::Begin("Properties")) {
		ImGui::End();
		return;
	}

	// Draw Menu
	if (ImGui::BeginMenuBar()) {
		DrawAddComponentMenu(device, resource_mgr);
		ImGui::EndMenuBar();
	}


	//----------------------------------------------------------------------------------
	// Name/Details
	//----------------------------------------------------------------------------------
	ImGui::InputText("", &entity.getName());
	ImGui::SameLine();
	bool state = entity.isActive();
	if (ImGui::Checkbox("Active", &state))
		entity.setActive(state);

	ImGui::Separator();

	ImGui::Text("Index:   %d", entity.getPtr().getHandle().index);
	ImGui::Text("Counter: %d", entity.getPtr().getHandle().counter);
	ImGui::Spacing();


	//----------------------------------------------------------------------------------
	// Parent
	//----------------------------------------------------------------------------------
	static const std::string none = "None";
	static std::vector<std::reference_wrapper<const std::string>> names;
	names.clear();
	names.push_back(std::cref(none));
	for (const auto& ptr : entities) {
		names.push_back(ptr->getName());
	}

	static const auto getter = [](void* data, int idx, const char** out_text) -> bool {
		auto& vector = *static_cast<const std::vector<DXGI_MODE_DESC>*>(data);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = names[idx].get().c_str();
		return true;
	};

	static int index = 0;
	if (ImGui::Combo("Parent", &index, getter, names.data(), static_cast<int>(names.size()))) {
		if (index != 0) {
			entities[index - 1]->addChild(entity.getPtr()); //subtract 1 since index 0 is "None"
		} else if (entity.hasParent()) {
			entity.getParent()->removeChild(entity.getPtr());
		}
	}


	//----------------------------------------------------------------------------------
	// Draw Component Nodes
	//----------------------------------------------------------------------------------


	// Transform
	if (auto* transform = entity.getComponent<Transform>()) {
		DrawComponentNode("Transform", *transform);

		// Draw transform manipulation tool. Only for the primary camera since it
		// doesn't work well when drawing multiple transform tools at once.
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

	// Model Root
	if (entity.hasComponent<ModelRoot>()) {
		auto roots = entity.getAll<ModelRoot>();
		for (auto* root : roots) {
			DrawComponentNode(("ModelRoot: " + root->getName()).c_str(), *root);
		}
	}

	// Perspective Camera
	if (entity.hasComponent<PerspectiveCamera>()) {
		auto cams = entity.getAll<PerspectiveCamera>();
		for (auto* cam : cams) {
			DrawComponentNode("Perspective Camera", *cam);
		}
	}

	// Orthographic Camera
	if (entity.hasComponent<OrthographicCamera>()) {
		auto cams = entity.getAll<OrthographicCamera>();
		for (auto* cam : cams) {
			DrawComponentNode("Orthographic Camera", *cam);
		}
	}

	// Text
	if (entity.hasComponent<Text>()) {
		auto texts = entity.getAll<Text>();
		for (auto* text : texts) {
			DrawComponentNode("Text", *text);
		}
	}

	// Ambient Light
	if (entity.hasComponent<AmbientLight>()) {
		auto lights = entity.getAll<AmbientLight>();
		for (auto* light : lights) {
			DrawComponentNode("Ambient Light", *light);
		}
	}

	// Directional Light
	if (entity.hasComponent<DirectionalLight>()) {
		auto lights = entity.getAll<DirectionalLight>();
		for (auto* light : lights) {
			DrawComponentNode("Directional Light", *light);
		}
	}

	// Point Light
	if (entity.hasComponent<PointLight>()) {
		auto lights = entity.getAll<PointLight>();
		for (auto* light : lights) {
			DrawComponentNode("Point Light", *light);
		}
	}

	// Spot Light
	if (entity.hasComponent<SpotLight>()) {
		auto lights = entity.getAll<SpotLight>();
		for (auto* light : lights) {
			DrawComponentNode("Spot Light", *light);
		}
	}

	// Camera Movement
	if (entity.hasComponent<CameraMovement>()) {
		auto components = entity.getAll<CameraMovement>();
		for (auto* comp : components) {
			DrawComponentNode("Camera Movement", *comp);
		}
	}

	// Mouse Rotation
	if (entity.hasComponent<MouseRotation>()) {
		auto components = entity.getAll<MouseRotation>();
		for (auto* comp : components) {
			DrawComponentNode("Mouse Rotation", *comp);
		}
	}

	// Axis Rotation
	if (entity.hasComponent<AxisRotation>()) {
		auto components = entity.getAll<AxisRotation>();
		for (auto* comp : components) {
			DrawComponentNode("Axis Rotation", *comp);
		}
	}

	// Axis Orbit
	if (entity.hasComponent<AxisOrbit>()) {
		auto components = entity.getAll<AxisOrbit>();
		for (auto* comp : components) {
			DrawComponentNode("Axis Orbit", *comp);
		}
	}

	ImGui::End(); //"Properties"
}




//----------------------------------------------------------------------------------
//
//   Scene Tree
//
//----------------------------------------------------------------------------------

// Draw a single tree node
void DrawEntityNode(EntityPtr entity_ptr) {

	if (!entity_ptr.valid())
		return;

	auto* entity = entity_ptr.get();
	auto handle = entity_ptr.getHandle();

	//----------------------------------------------------------------------------------
	// Draw Entity Node
	//----------------------------------------------------------------------------------
	bool node_open = true;
	if (entity->hasChildren()) {
		node_open = g_scene_tree.newNode(entity_ptr, entity->getName().c_str());
	} else {
		node_open = g_scene_tree.newLeafNode(entity_ptr, entity->getName().c_str());
	}

	if (node_open) {
		// Draw any child entities in this node
		if (entity->hasChildren()) {
			entity->forEachChild([](EntityPtr& child) {
				DrawEntityNode(child);
			});
		}

		g_scene_tree.endNode();
	}
}


// Draw the scene tree
void DrawTree(Engine& engine) {

	auto& scene = engine.getScene();

	if (ImGui::BeginChild("Object List", {0, 0}, false, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("%s (Entities: %llu)", scene.getName().c_str(), scene.getEntities().size());
		ImGui::Separator();

		// Draw a tree node for each root entity
		auto& entities = scene.getEntities();
		for (const auto& entity_ptr : entities) {
			if (entity_ptr->hasParent()) continue;
			DrawEntityNode(entity_ptr);
		}

		if (auto selected = g_scene_tree.getSelected(); selected.valid()) {
			DrawEntityDetails(*selected, engine);
		}
	}

	ImGui::EndChild();
}




//----------------------------------------------------------------------------------
//
//   Scene Tree Menu
//
//----------------------------------------------------------------------------------


void DrawEntityMenu(Scene& scene) {

	if (ImGui::BeginMenu("Entity")) {

		if (ImGui::MenuItem("New")) {
			scene.addEntity();
		}

		if (ImGui::BeginMenu("Selected", g_scene_tree.getSelected().valid())) {
			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) {
				scene.removeEntity(g_scene_tree.getSelected()->getPtr());
			}

			ImGui::EndMenu(); //Selected
		}

		ImGui::EndMenu(); //Entity
	}
}


void DrawSceneMenu(Scene& scene) {

	if (ImGui::BeginMenuBar()) {
		DrawEntityMenu(scene);
		ImGui::EndMenuBar();
	}
}




//----------------------------------------------------------------------------------
//
//   System Menu
//
//----------------------------------------------------------------------------------

void DrawDisplaySettingsPopup(Engine& engine) {

	if (ImGui::BeginPopupModal("Display Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		auto& settings = engine.getRenderingMgr().getDisplayConfig();
		static auto current = static_cast<int>(settings.getDisplayDescIndex());

		// Display mode strings
		static std::vector<std::string> display_modes;
		if (display_modes.empty()) {
			for (const auto& desc : settings.getDisplayDescList()) {
				const auto exact_refresh = static_cast<f32>(desc.RefreshRate.Numerator) / static_cast<f32>(desc.RefreshRate.Denominator);
				const auto refresh = static_cast<u32>(round(exact_refresh));
				display_modes.push_back(std::to_string(desc.Width) + "x" + std::to_string(desc.Height) + " " + std::to_string(refresh) + "Hz");
			}
		}

		// Lamda to convert display mode to a string
		static const auto getter = [](void* data, int idx, const char** out_text) -> bool {
			auto& vector = *static_cast<const std::vector<DXGI_MODE_DESC>*>(data);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
			*out_text = display_modes[idx].c_str();
			return true;
		};

		ImGui::Combo("Display Mode",
		             &current,
		             getter,
		             static_cast<void*>(const_cast<std::vector<DXGI_MODE_DESC>*>(&settings.getDisplayDescList())),
		             static_cast<int>(settings.getDisplayDescList().size()));


		if (ImGui::Button("Apply")) {
			settings.setDisplayDesc(current);
			engine.requestResize();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}


void DrawEngineSettingsPopup(Engine& engine) {

	if (ImGui::BeginPopupModal("Engine Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		auto& rendering_mgr = engine.getRenderingMgr();
		auto& rendering_config = rendering_mgr.getRenderingConfig();

		u32 smap_res = rendering_config.getShadowMapRes();
		if (ImGui::DragScalar("Shadow Map Resolution", ImGuiDataType_U32, &smap_res, 1)) {
			rendering_config.setShadowMapRes(smap_res);
		}

		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}


void DrawSystemMenu(Engine& engine) {

	bool display_settings_popup = false;
	bool engine_settings_popup = false;

	// Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("System")) {

			if (ImGui::MenuItem("Display Settings")) {
				display_settings_popup = true;
			}

			if (ImGui::MenuItem("Engine Settings")) {
				engine_settings_popup = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit")) {
				engine.requestExit();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (display_settings_popup) {
		ImGui::OpenPopup("Display Settings");
	}
	if (engine_settings_popup) {
		ImGui::OpenPopup("Engine Settings");
	}


	// Settings Popups
	DrawDisplaySettingsPopup(engine);
	DrawEngineSettingsPopup(engine);
}




//----------------------------------------------------------------------------------
//
//   Metrics
//
//----------------------------------------------------------------------------------
void DrawMetrics(Engine& engine) {

	static constexpr auto si_prefix = MetricsGuiMetric::USE_SI_UNIT_PREFIX;
	static constexpr auto known_min_max = MetricsGuiMetric::KNOWN_MIN_VALUE | MetricsGuiMetric::KNOWN_MAX_VALUE;

	// Frame Time & FPS
	static MetricsGuiPlot frame_plot;
	static MetricsGuiMetric frame_time("Frame Time", "s", si_prefix);
	static MetricsGuiMetric fps("FPS", "", 0);

	// CPU Usage
	static MetricsGuiPlot cpu_plot;
	static MetricsGuiMetric total_cpu("Total Usage", "%", known_min_max);
	static MetricsGuiMetric process_cpu("Process Usage", "%", known_min_max);

	// RAM Usage
	static MetricsGuiPlot ram_plot;
	static MetricsGuiMetric total_ram("Total Usage", "B", si_prefix | known_min_max);
	static MetricsGuiMetric process_ram("Process Usage", "B", si_prefix | known_min_max);

	// GPU Time
	static MetricsGuiPlot gpu_plot;
	static MetricsGuiMetric frame_gpu_time("Frame Time (GPU)", "s", si_prefix);
	static MetricsGuiMetric imgui_render("ImGui Render", "s", si_prefix);

	// Scene Render GPU Time
	static MetricsGuiPlot scene_gpu_plot;
	static MetricsGuiMetric scene_render("Scene Render", "s", si_prefix);
	static MetricsGuiMetric skybox_render("Skybox Render", "s", si_prefix);
	static MetricsGuiMetric shadow_maps("Shadow Maps Render", "s", si_prefix);
	static MetricsGuiMetric forward_render("Forward Render", "s", si_prefix);
	static MetricsGuiMetric text_render("Text Render", "s", si_prefix);

	for (static bool once = true; once; once = false) {
		frame_plot.mShowInlineGraphs = true;
		frame_plot.mInlinePlotRowCount = 3;
		frame_plot.mShowLegendAverage = true;
		frame_plot.mShowLegendColor = false;
		frame_plot.mShowLegendDesc = false;
		frame_time.mSelected = true;
		fps.mSelected = true;
		frame_plot.AddMetric(&fps);
		frame_plot.AddMetric(&frame_time);

		total_cpu.mKnownMinValue = 0.0f;
		total_cpu.mKnownMaxValue = 100.0f;
		process_cpu.mKnownMinValue = 0.0f;
		process_cpu.mKnownMaxValue = 100.0f;
		cpu_plot.mShowLegendMin = false;
		cpu_plot.mShowLegendMax = false;
		cpu_plot.mShowLegendAverage = true;
		cpu_plot.AddMetric(&total_cpu);
		cpu_plot.AddMetric(&process_cpu);

		total_ram.mKnownMinValue = 0.0f;
		total_ram.mKnownMaxValue = static_cast<float>(engine.getSysMon().memory().getPhysicalMemSize());
		process_ram.mKnownMinValue = 0.0f;
		process_ram.mKnownMaxValue = static_cast<float>(engine.getSysMon().memory().getPhysicalMemSize());
		ram_plot.mShowLegendAverage = true;
		ram_plot.mShowLegendMin = false;
		ram_plot.AddMetric(&total_ram);
		ram_plot.AddMetric(&process_ram);

		gpu_plot.mShowLegendMin = false;
		gpu_plot.mShowLegendMax = false;
		gpu_plot.mShowLegendAverage = true;
		gpu_plot.AddMetric(&frame_gpu_time);
		gpu_plot.AddMetric(&scene_render);
		gpu_plot.AddMetric(&imgui_render);
		gpu_plot.LinkLegends(&scene_gpu_plot);

		scene_gpu_plot.mShowLegendMin = false;
		scene_gpu_plot.mShowLegendMax = false;
		scene_gpu_plot.mShowLegendAverage = true;
		scene_gpu_plot.AddMetric(&skybox_render);
		scene_gpu_plot.AddMetric(&shadow_maps);
		scene_gpu_plot.AddMetric(&forward_render);
		scene_gpu_plot.AddMetric(&text_render);
	}


	//----------------------------------------------------------------------------------
	// Update frame time
	//----------------------------------------------------------------------------------
	const auto& timer = engine.getTimer();
	frame_time.AddNewValue(static_cast<float>(timer.deltaTime()));
	fps.AddNewValue(static_cast<float>(1.0 / timer.deltaTime()));
	frame_plot.UpdateAxes();


	//----------------------------------------------------------------------------------
	// Update CPU/RAM usage
	//----------------------------------------------------------------------------------
	const auto& sys_mon = engine.getSysMon();
	total_cpu.AddNewValue(static_cast<float>(sys_mon.cpu().getTotalCpuPercentage()));
	process_cpu.AddNewValue(static_cast<float>(sys_mon.cpu().getProcessCpuPercentage()));
	cpu_plot.UpdateAxes();

	total_ram.AddNewValue(static_cast<float>(sys_mon.memory().getTotalUsedPhysicalMem()));
	process_ram.AddNewValue(static_cast<float>(sys_mon.memory().getProcessUsedPhysicalMem()));
	ram_plot.UpdateAxes();


	//----------------------------------------------------------------------------------
	// Update GPU time
	//----------------------------------------------------------------------------------
	const auto& profiler = engine.getRenderingMgr().getProfiler();
	frame_gpu_time.AddNewValue(profiler.deltaTime(GPUTimestamps::frame));
	imgui_render.AddNewValue(profiler.deltaTime("ImGui"));
	scene_render.AddNewValue(profiler.deltaTime(GPUTimestamps::render_scene));
	gpu_plot.UpdateAxes();

	skybox_render.AddNewValue(profiler.deltaTime("Skybox"));
	shadow_maps.AddNewValue(profiler.deltaTime(GPUTimestamps::shadow_maps));
	forward_render.AddNewValue(profiler.deltaTime(GPUTimestamps::forward_render));
	text_render.AddNewValue(profiler.deltaTime("Text"));
	scene_gpu_plot.UpdateAxes();


	//----------------------------------------------------------------------------------
	// Draw Window
	//----------------------------------------------------------------------------------
	ImGui::SetNextWindowSizeConstraints({500, 0}, {500, -1});
	if (ImGui::Begin("Metrics", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::BeginTabBar("Metrics Tab Bar")) {

			if (ImGui::BeginTabItem("All")) {
				ImGui::Text("FPS");
				ImGui::Separator();
				frame_plot.DrawList();

				ImGui::Spacing();
				ImGui::Text("CPU");
				ImGui::Separator();
				cpu_plot.DrawHistory();

				ImGui::Spacing();
				ImGui::Text("RAM");
				ImGui::Separator();
				ram_plot.DrawHistory();

				ImGui::Spacing();
				ImGui::Text("GPU");
				ImGui::Separator();
				gpu_plot.DrawHistory();
				//scene_gpu_plot.DrawHistory();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("FPS")) {
				frame_plot.DrawList();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("CPU")) {
				cpu_plot.DrawHistory();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("RAM")) {
				ram_plot.DrawHistory();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("GPU")) {
				gpu_plot.DrawHistory();
				ImGui::Spacing();
				ImGui::Text("Scene Render Breakdown");
				scene_gpu_plot.DrawHistory();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}




//----------------------------------------------------------------------------------
//
//   UserInterface
//
//----------------------------------------------------------------------------------


void UserInterface::update(Engine& engine) {

	//ImGui::ShowDemoWindow();
	ImGuizmo::BeginFrame(); //technicall should be called right after ImGui_XXXX_NewFrame();

	auto& device = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto& scene = engine.getScene();

	// Setup window layout
	ImGui::SetNextWindowSize(ImVec2(275, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(375, 425), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::End();

	// Draw the system menu
	static bool system_menu = true;
	if (engine.getInput().isKeyPressed(Keyboard::F3)) {
		system_menu = !system_menu;
	}
	if (system_menu) {
		DrawSystemMenu(engine);
	}

	// Draw the system metrics
	DrawMetrics(engine);

	// Draw scene window contents
	if (ImGui::Begin("Scene")) {
		// Draw menu
		DrawSceneMenu(scene);

		// Process model popup
		NewModelMenu::ProcNewModelPopup(device, resource_mgr, g_scene_tree.getSelected());

		// Draw tree
		DrawTree(engine);
	}
	ImGui::End();
}
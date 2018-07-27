#include "stdafx.h"
#include "user_interface.h"

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"

#include "imgui.h"
#include "engine/engine.h"
#include "scene/scene.h"
#include "io/io.h"
#include "log/log.h"
#include "buffer/buffers.h"
#include "resource/model/blueprint_factory.h"


void* selected = nullptr;
handle64 selected_entity{};


enum class ModelType {
	None,
	Cube,
	Box,
	Sphere,
	GeoSphere,
	Cylinder,
	Torus,
	Cone,
	Tetrahedron,
	Octahedron,
	Dodecahedron,
	Icosahedron
};




//----------------------------------------------------------------------------------
//
//   Object Details
//
//----------------------------------------------------------------------------------

template<typename T>
void DrawComponentState(T& component) {
	ImGui::Text("State");
	ImGui::Separator();
	bool state = component.isActive();
	if (ImGui::Checkbox("Active", &state))
		component.setActive(state);
	ImGui::Spacing();
}


void DrawDetails(Scene& scene) {

	ImGui::Text(scene.getName().c_str());
	ImGui::Separator();

	std::string entity_count = "Entities: " + std::to_string(scene.getEntities().size());
	ImGui::Text(entity_count.c_str());
}


void DrawDetails(Transform& transform) {

	DrawComponentState(transform);

	ImGui::Text("Transform");
	ImGui::Separator();

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

	ImGui::Text("Settings");
	ImGui::Separator();

	//----------------------------------------------------------------------------------
	// Render Mode
	//----------------------------------------------------------------------------------
	static constexpr const char* render_mode_names[] = {
		"Forward",
		"Forward+",
		"Deferred"
	};
	static constexpr RenderMode render_modes[] = {
		RenderMode::Forward,
		RenderMode::ForwardPlus,
		RenderMode::Deferred
	};
	static_assert(std::size(render_mode_names) == std::size(render_modes));

	auto render_mode = static_cast<int>(settings.getRenderMode());
	if (ImGui::Combo("Render Mode", &render_mode, render_mode_names, std::size(render_mode_names)))
		settings.setRenderMode(static_cast<RenderMode>(render_mode));


	//----------------------------------------------------------------------------------
	// Lighting Mode
	//----------------------------------------------------------------------------------
	static constexpr const char* light_mode_names[] = {
		"Default",
		"Unlint",
		"FalseColorPosition",
		"FalseColorNormal",
		"FalseColorDepth"
	};
	static constexpr LightingMode light_modes[] = {
		LightingMode::Default,
		LightingMode::Unlit,
		LightingMode::FalseColorPosition,
		LightingMode::FalseColorNormal,
		LightingMode::FalseColorDepth
	};
	static_assert(std::size(light_mode_names) == std::size(light_modes));

	auto light_mode = static_cast<int>(settings.getLightingMode());
	if (ImGui::Combo("Lighting Mode", &light_mode, light_mode_names, std::size(light_mode_names)))
		settings.setLightingMode(static_cast<LightingMode>(light_mode));


	//----------------------------------------------------------------------------------
	// Render Options
	//----------------------------------------------------------------------------------
	ImGui::Spacing();

	auto bounding_volumes = settings.hasRenderOption(RenderOptions::BoundingVolume);
	if (ImGui::Checkbox("Bounding Volumes", &bounding_volumes))
		settings.toggleRenderOption(RenderOptions::BoundingVolume);

	auto wireframe = settings.hasRenderOption(RenderOptions::Wireframe);
	if (ImGui::Checkbox("Wireframe", &wireframe))
		settings.toggleRenderOption(RenderOptions::Wireframe);


	//----------------------------------------------------------------------------------
	// Fog
	//----------------------------------------------------------------------------------
	ImGui::Spacing();

	auto& fog = settings.getFog();
	ImGui::ColorEdit3("Fog Color", fog.color.data());
	ImGui::DragFloat("Fog Start", &fog.start, 0.1f);
	ImGui::DragFloat("Fog Range", &fog.range, 0.1f);
}


void DrawDetails(PerspectiveCamera& camera) {

	DrawComponentState(camera);

	ImGui::Text("Camera");
	ImGui::Separator();

	// FOV
	f32 fov = camera.getFOV();
	if (ImGui::DragFloat("FOV", &fov, 0.01f, XM_PI / 8.0f, XM_2PI / 3.0f)) {
		camera.setFOV(fov);
	}

	// Viewport
	auto& vp = camera.getViewport();
	vec2_u32 size = vp.getSize();
	vec2_u32 pos  = vp.getTopLeft();

	static constexpr u32 v_min_size = 1;
	static constexpr u32 v_min_pos  = 0;
	static constexpr u32 v_max      = 15360;

	if (ImGui::DragScalarN("Viewport Size", ImGuiDataType_U32, size.data(), 2, 1.0f, &v_min_size, &v_max)) {
		vp.setSize(size);
	}
	if (ImGui::DragScalarN("Viewport Top-Left", ImGuiDataType_U32, pos.data(), 2, 1.0f, &v_min_pos, &v_max)) {
		vp.setTopLeft(pos);
	}

	// Depth
	vec2_f32 depth = camera.getZDepth();
	if (ImGui::DragFloat2("Depth Range", depth.data(), 1.0f, 0.01f, FLT_MAX)) {
		camera.setZDepth(depth);
	}


	DrawCameraSettings(camera.getSettings());
}


void DrawDetails(OrthographicCamera& camera) {

	DrawComponentState(camera);

	ImGui::Text("Camera");
	ImGui::Separator();
	
	// Ortho Size
	auto ortho_size = camera.getSize();
	if (ImGui::DragFloat2("Orthographic Size", ortho_size.data(), 0.01f, 1.0f, 100.0f)) {
		camera.setSize(ortho_size);
	}

	// Viewport
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

	// Depth
	vec2_f32 depth = camera.getZDepth();
	if (ImGui::DragFloat2("Depth Range", depth.data(), 1.0f, 0.01f, FLT_MAX))
		camera.setZDepth(depth);


	DrawCameraSettings(camera.getSettings());
}


void DrawDetails(CameraMovement& movement) {

	DrawComponentState(movement);

	ImGui::Text("Camera Movement");
	ImGui::Separator();

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

	ImGui::Text("Mouse Rotation");
	ImGui::Separator();

	f32 sensitivity = rotation.getSensitivity();

	if (ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f, 0.0f, FLT_MAX)) {
		rotation.setSensitivity(sensitivity);
	}
}


void DrawDetails(Model& model) {

	DrawComponentState(model);
}


void DrawDetails(ModelChild& child) {

	ImGui::Text(child.getName().c_str());
	ImGui::Separator();

	// Change material properties, textures
	std::string name = "Material - " + child.getMaterial().name;
	ImGui::Text(name.c_str());

	ImGui::ColorEdit3("Diffuse Color", (f32*)child.getMaterial().diffuse.data());
	ImGui::ColorEdit3("Ambient Color", (f32*)child.getMaterial().ambient.data());
	ImGui::ColorEdit3("Specular Color", (f32*)child.getMaterial().specular.data());
	ImGui::DragFloat("Specular Exponent", (f32*)&child.getMaterial().specular.w, 0.01f, 0.0f, FLT_MAX);
	ImGui::ColorEdit3("Reflective Color", (f32*)child.getMaterial().reflect.data());
	ImGui::Checkbox("Reflection", (bool*)&child.getMaterial().reflection_enabled);
}


void DrawDetails(DirectionalLight& light) {

	DrawComponentState(light);

	ImGui::Text("Directional Light");
	ImGui::Separator();

	auto diffuse = light.getDiffuseColor();
	if (ImGui::ColorEdit3("Diffuse Color", diffuse.data()))
		light.setDiffuseColor(diffuse);

	auto ambient = light.getAmbientColor();
	if (ImGui::ColorEdit3("Ambient Color", ambient.data()))
		light.setAmbientColor(ambient);

	auto specular = light.getSpecular();
	if (ImGui::ColorEdit3("Specular Color", specular.data()))
		light.setSpecular(specular);

	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.1f, FLT_MAX))
		light.setRange(range);

	auto proj_size = light.getSize();
	if (ImGui::DragFloat2("Size", proj_size.data(), 0.01f, 0.01f, FLT_MAX))
		light.setSize(proj_size);

	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(PointLight& light) {

	DrawComponentState(light);

	ImGui::Text("Point Light");
	ImGui::Separator();

	f32 range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, FLT_MAX))
		light.setRange(range);

	auto attenuation = light.getAttenuation();
	if (ImGui::DragFloat3("Attenuation", attenuation.data(), 0.01f, 0.0f, 1.0f))
		light.setAttenuation(attenuation);

	auto diffuse = light.getDiffuseColor();
	if (ImGui::ColorEdit3("Diffuse Color", diffuse.data()))
		light.setDiffuseColor(diffuse);

	auto ambient = light.getAmbientColor();
	if (ImGui::ColorEdit3("Ambient Color", ambient.data()))
		light.setAmbientColor(ambient);

	auto specular = light.getSpecular();
	if (ImGui::ColorEdit3("Specular Color", specular.data()))
		light.setSpecular(specular);

	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(SpotLight& light) {

	DrawComponentState(light);

	ImGui::Text("Spot Light");
	ImGui::Separator();

	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, FLT_MAX))
		light.setRange(range);

	auto umbra = light.getUmbraAngle();
	if (ImGui::DragFloat("Umbra", &umbra, 0.01f, 0.01f, XM_PIDIV2))
		light.setUmbraAngle(umbra);

	auto penumbra = light.getPenumbraAngle();
	if (ImGui::DragFloat("Penumbra", &penumbra, 0.01f, 0.01f, XM_PIDIV2))
		light.setPenumbraAngle(penumbra);

	auto attenuation = light.getAttenuation();
	if (ImGui::DragFloat3("Attenuation", attenuation.data(), 0.01f, 0.0f, 1.0f))
		light.setAttenuation(attenuation);

	auto diffuse = light.getDiffuseColor();
	if (ImGui::ColorEdit3("Diffuse Color", diffuse.data()))
		light.setDiffuseColor(diffuse);

	auto ambient = light.getAmbientColor();
	if (ImGui::ColorEdit3("Ambient Color", ambient.data()))
		light.setAmbientColor(ambient);

	auto specular = light.getSpecular();
	if (ImGui::ColorEdit3("Specular Color", specular.data()))
		light.setSpecular(specular);

	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(AxisRotation& rotation) {

	DrawComponentState(rotation);

	ImGui::Text("Axis Rotation");
	ImGui::Separator();


	// X Rotation
	static bool x_enable = rotation.hasAxis(AxisRotation::Axis::X);

	if (ImGui::Checkbox("X Axis", &x_enable)) {
		if (x_enable) rotation.addAxis(AxisRotation::Axis::X);
		else rotation.removeAxis(AxisRotation::Axis::X);
	}
	if (x_enable) {
		f32 speed_x = rotation.getSpeedX();
		if (ImGui::DragFloat("X Speed", &speed_x, 0.05f, -FLT_MAX, FLT_MAX)) {
			rotation.setSpeedX(speed_x);
		}
	}


	// Y Rotation
	static bool y_enable = rotation.hasAxis(AxisRotation::Axis::Y);

	if (ImGui::Checkbox("Y Axis", &y_enable)) {
		if (y_enable) rotation.addAxis(AxisRotation::Axis::Y);
		else rotation.removeAxis(AxisRotation::Axis::Y);
	}
	if (y_enable) {
		f32 speed_y = rotation.getSpeedY();
		if (ImGui::DragFloat("Y Speed", &speed_y, 0.05f, -FLT_MAX, FLT_MAX)) {
			rotation.setSpeedY(speed_y);
		}
	}


	// Z Rotation
	static bool z_enable = rotation.hasAxis(AxisRotation::Axis::Z);

	if (ImGui::Checkbox("Z Axis", &z_enable)) {
		if (z_enable) rotation.addAxis(AxisRotation::Axis::Z);
		else rotation.removeAxis(AxisRotation::Axis::Z);
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

	ImGui::Text("Axis Orbit");
	ImGui::Separator();

	auto axis = XMStore<vec3_f32>(orbit.getAxis());
	if (ImGui::InputFloat3("Axis", axis.data())) {
		orbit.setAxis(axis);
	}
}




//----------------------------------------------------------------------------------
//
//   Object Tree
//
//----------------------------------------------------------------------------------

template<typename T>
void DrawDetailsPanel(T& item) {
	ImGui::Begin("Properties");
	DrawDetails(item);
	ImGui::End();
}


template<typename T>
void DrawNode(const char* text, T& item) {
	bool node_selected = (selected == &item);
	ImGui::Selectable(text, &node_selected);

	if (ImGui::IsItemClicked()) {
		selected = &item;
	}
	if (node_selected) {
		DrawDetailsPanel(item);
	}
}


void DrawTreeNodes(ECS& ecs_engine, Scene& scene) {

	auto& entities = scene.getEntities();

	for (const auto entity : entities) {

		std::string name = "Entity (index: " + std::to_string(entity.index) + ", counter: " + std::to_string(entity.counter) + ")";

		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		                                 | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                                 | ((selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0);

		const bool node_open = ImGui::TreeNodeEx(name.c_str(), flags, name.c_str());

		if (ImGui::IsItemClicked())
			selected_entity = entity;

		if (!node_open) continue;

		// Transform
		if (auto* transform = ecs_engine.getComponent<Transform>(entity)) {
			DrawNode("Transform", *transform);
		}

		// Perspective Camera
		if (auto* cam = ecs_engine.getComponent<PerspectiveCamera>(entity)) {
			DrawNode("Perspective Camera", *cam);
		}

		// Orthographic Camera
		if (auto* cam = ecs_engine.getComponent<OrthographicCamera>(entity)) {
			DrawNode("Orthographic Camera", *cam);
		}

		// Model
		if (auto* model = ecs_engine.getComponent<Model>(entity)) {
			const bool node_selected = (selected == model);

			const ImGuiTreeNodeFlags model_flags = ImGuiTreeNodeFlags_OpenOnArrow
			                                       | ImGuiTreeNodeFlags_OpenOnDoubleClick
			                                       | (node_selected ? ImGuiTreeNodeFlags_Selected : 0);

			const bool open = ImGui::TreeNodeEx("Model", model_flags);

			if (ImGui::IsItemClicked())
				selected = model;

			if (open) {
				model->forEachChild([&](ModelChild& child) {
					DrawNode(child.getName().c_str(), child);
				});

				ImGui::TreePop();
			}

			if (node_selected) {
				DrawDetailsPanel(*model);
			}
		}

		// Directional Light
		if (auto* light = ecs_engine.getComponent<DirectionalLight>(entity)) {
			DrawNode("Directional Light", *light);
		}

		// Point Light
		if (auto* light = ecs_engine.getComponent<PointLight>(entity)) {
			DrawNode("Point Light", *light);
		}

		// Spot Light
		if (auto* light = ecs_engine.getComponent<SpotLight>(entity)) {
			DrawNode("Spot Light", *light);
		}

		// Camera Movement
		if (auto* cam_movement = ecs_engine.getComponent<CameraMovement>(entity)) {
			DrawNode("Camera Movement", *cam_movement);
		}

		// Mouse Rotation
		if (auto* mouse_rotation = ecs_engine.getComponent<MouseRotation>(entity)) {
			DrawNode("Mouse Rotation", *mouse_rotation);
		}

		// Axis Rotation
		if (auto* axis_rotation = ecs_engine.getComponent<AxisRotation>(entity)) {
			DrawNode("Axis Rotation", *axis_rotation);
		}

		// Axis Orbit
		if (auto* axis_orbit = ecs_engine.getComponent<AxisOrbit>(entity)) {
			DrawNode("Axis Orbit", *axis_orbit);
		}

		ImGui::TreePop();
	}

	ImGui::TreePop();
}


void DrawTree(ECS& ecs_engine, Scene& scene) {

	if (ImGui::BeginChild("object list", ImVec2(250, 0))) {

		const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		                                      | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                                      | ((selected == &scene) ? ImGuiTreeNodeFlags_Selected : 0);

		const bool node_open = ImGui::TreeNodeEx(&scene, node_flags, scene.getName().c_str());

		if (ImGui::IsItemClicked())
			selected = &scene;

		if (node_open) {
			DrawTreeNodes(ecs_engine, scene);
		}

		if (selected == &scene) {
			DrawDetailsPanel(scene);
		}
	}

	ImGui::EndChild();
}




//----------------------------------------------------------------------------------
//
//   Popup Windows
//
//----------------------------------------------------------------------------------

void ProcNewModelPopups(ID3D11Device& device,
                        ECS& ecs_engine,
                        ResourceMgr& resource_mgr,
                        handle64 entity,
                        ModelType type) {

	switch (type) {
		case ModelType::Cube:
			ImGui::OpenPopup("New Cube");
			break;
		case ModelType::Box:
			ImGui::OpenPopup("New Box");
			break;
		case ModelType::Sphere:
			ImGui::OpenPopup("New Sphere");
			break;
		case ModelType::GeoSphere:
			ImGui::OpenPopup("New GeoSphere");
			break;
		case ModelType::Cylinder:
			ImGui::OpenPopup("New Cylinder");
			break;
		case ModelType::Torus:
			ImGui::OpenPopup("New Torus");
			break;
		case ModelType::Cone:
			ImGui::OpenPopup("New Cone");
			break;
		case ModelType::Tetrahedron:
			ImGui::OpenPopup("New Tetrahedron");
			break;
		case ModelType::Octahedron:
			ImGui::OpenPopup("New Octahedron");
			break;
		case ModelType::Dodecahedron:
			ImGui::OpenPopup("New Dodecahedron");
			break;
		case ModelType::Icosahedron:
			ImGui::OpenPopup("New Icosahedron");
			break;
		default:
			break;
	}


	if (ImGui::BeginPopupModal("New Cube", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size     = 1.0f;
		static bool rhcoords = false;
		static bool invertn  = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);
		ImGui::Checkbox("Invert Normals", &invertn);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateCube<VertexPositionNormalTexture>(resource_mgr, size, rhcoords, invertn);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Box", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static vec3_f32 size     = { 1.0f, 1.0f, 1.0f };
		static bool     rhcoords = false;
		static bool     invertn  = false;

		ImGui::InputFloat3("Size", size.data());
		ImGui::Checkbox("Right-hand Coords", &rhcoords);
		ImGui::Checkbox("Invert Normals", &invertn);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateBox<VertexPositionNormalTexture>(resource_mgr, size, rhcoords, invertn);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Sphere", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32    diameter     = 1.0f;
		static size_t tessellation = 16;
		static bool   rhcoords     = false;
		static bool   invertn      = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);
		ImGui::Checkbox("Invert Normals", &invertn);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 64) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateSphere<VertexPositionNormalTexture>(resource_mgr, diameter, tessellation, rhcoords, invertn);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New GeoSphere", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32    diameter     = 1.0f;
		static size_t tessellation = 3;
		static bool   rhcoords     = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 6) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is extremely high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateGeoSphere<VertexPositionNormalTexture>(resource_mgr, diameter, tessellation, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cylinder", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32    height       = 1.0f;
		static f32    diameter     = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords     = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCylinder<VertexPositionNormalTexture>(resource_mgr, diameter, height, tessellation, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cone", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32    height       = 1.0f;
		static f32    diameter     = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords     = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCone<VertexPositionNormalTexture>(resource_mgr, diameter, height, tessellation, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Torus", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  thickness = 0.333f;
		static f32  diameter = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords = false;

		ImGui::InputFloat("Thickness", &thickness);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateTorus<VertexPositionNormalTexture>(resource_mgr, diameter, thickness, tessellation, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Tetrahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size     = 1.0f;
		static bool rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateTetrahedron<VertexPositionNormalTexture>(resource_mgr, size, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Octahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size     = 1.0f;
		static bool rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateOctahedron<VertexPositionNormalTexture>(resource_mgr, size, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Dodecahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size     = 1.0f;
		static bool rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateDodecahedron<VertexPositionNormalTexture>(resource_mgr, size, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Icosahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size     = 1.0f;
		static bool rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateIcosahedron<VertexPositionNormalTexture>(resource_mgr, size, rhcoords);
			ecs_engine.addComponent<Model>(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}




//----------------------------------------------------------------------------------
//
//   Menu
//
//----------------------------------------------------------------------------------

void DrawMenu(ID3D11Device& device,
              ECS& ecs_engine,
              ResourceMgr& resource_mgr,
              Scene& scene,
              ModelType& add_model_popup) {

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("Entity")) {

			if (ImGui::MenuItem("New")) {
				scene.addEntity<WorldObject<>>();
			}

			if (ImGui::BeginMenu("Selected")) {

				if (ImGui::BeginMenu("Add Component")) {

					if (ImGui::MenuItem("Orthographic Camera")) {
						ecs_engine.addComponent<OrthographicCamera>(selected_entity, device, vec2_u32{480, 480});
					}

					if (ImGui::MenuItem("Perspective Camera")) {
						ecs_engine.addComponent<PerspectiveCamera>(selected_entity, device, vec2_u32{480, 480});
					}

					if (ImGui::MenuItem("Directional Light")) {
						ecs_engine.addComponent<DirectionalLight>(selected_entity);
					}

					if (ImGui::MenuItem("Point Light")) {
						ecs_engine.addComponent<PointLight>(selected_entity);
					}

					if (ImGui::MenuItem("Spot Light")) {
						ecs_engine.addComponent<SpotLight>(selected_entity);
					}

					if (ImGui::BeginMenu("Model")) {

						if (ImGui::MenuItem("From file")) {
							wchar_t szFile[512] = {};

							if (OpenFilePicker(szFile, sizeof(szFile))) {
								auto bp = resource_mgr.getOrCreate<ModelBlueprint>(szFile);
								ecs_engine.addComponent<Model>(selected_entity, device, bp);
							}
							else Logger::log(LogLevel::err, "Failed to open file dialog");
						}

						if (ImGui::BeginMenu("Geometric Shape")) {
							if (ImGui::MenuItem("Cube")) add_model_popup = ModelType::Cube;
							if (ImGui::MenuItem("Box")) add_model_popup = ModelType::Box;
							if (ImGui::MenuItem("Sphere")) add_model_popup = ModelType::Sphere;
							if (ImGui::MenuItem("GeoSphere")) add_model_popup = ModelType::GeoSphere;
							if (ImGui::MenuItem("Cylinder")) add_model_popup = ModelType::Cylinder;
							if (ImGui::MenuItem("Cone")) add_model_popup = ModelType::Cone;
							if (ImGui::MenuItem("Torus")) add_model_popup = ModelType::Torus;
							if (ImGui::MenuItem("Tetrahedron")) add_model_popup = ModelType::Tetrahedron;
							if (ImGui::MenuItem("Octahedron")) add_model_popup = ModelType::Octahedron;
							if (ImGui::MenuItem("Dodecahedron")) add_model_popup = ModelType::Dodecahedron;
							if (ImGui::MenuItem("Icosahedron")) add_model_popup = ModelType::Icosahedron;

							ImGui::EndMenu();
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Delete")) {
					if (selected_entity != handle64::invalid_handle)
						scene.removeEntity(selected_entity);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}





//----------------------------------------------------------------------------------
//
//   Draw
//
//----------------------------------------------------------------------------------

void UserInterface::draw(const Engine& engine) {

	auto& scene = engine.getScene();
	auto& ecs_engine = engine.getScene().getECS();
	auto& device = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();

	bool open = true;

	// Setup window layout
	ImGui::SetNextWindowSize(ImVec2(275, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene", &open, ImGuiWindowFlags_MenuBar);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(375, 425), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties", &open);
	ImGui::End();

	// Draw window contents
	if (ImGui::Begin("Scene")) {

		ModelType add_model_popup = ModelType::None;

		// Draw menu
		DrawMenu(device, ecs_engine, resource_mgr, scene, add_model_popup);

		// Process model popup
		ProcNewModelPopups(device, ecs_engine, resource_mgr, selected_entity, add_model_popup);

		// Draw tree
		DrawTree(ecs_engine, scene);
	}

	// End window
	ImGui::End();
}
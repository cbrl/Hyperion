#include "entity_details_window.h"
#include "engine/engine.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"


EntityDetailsWindow::EntityDetailsWindow() {
	ImGui::SetNextWindowSize(ImVec2{375, 425}, ImGuiCond_FirstUseEver);
	ImGui::Begin("Entity Properties", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::End();
}


void EntityDetailsWindow::draw(Engine& engine, EntityPtr entity_ptr) {

	auto&       device       = engine.getRenderingMgr().getDevice();
	auto&       resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto&       scene        = engine.getScene();
	const auto& entities     = scene.getEntities();

	// Begin window. Early return if window isn't open or entity isn't valid.
	if (!ImGui::Begin("Entity Properties", nullptr, ImGuiWindowFlags_MenuBar) || !entity_ptr) {
		ImGui::End();
		return;
	}

	// Get the entity
	Entity& entity = *entity_ptr;

	// Draw menu
	if (ImGui::BeginMenuBar()) {
		drawAddComponentMenu(engine, entity_ptr);
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
	entity_names_list.clear();
	entity_names_list.push_back(std::cref(none));
	for (const auto& ptr : entities) {
		entity_names_list.push_back(ptr->getName());
	}

	static const auto getter = [](void* data, int idx, const char** out_text) -> bool {
		auto& vector = *static_cast<std::vector<std::reference_wrapper<const std::string>>*>(data);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector[idx].get().c_str();
		return true;
	};

	if (ImGui::Combo("Parent", &entity_names_idx, getter, &entity_names_list, static_cast<int>(entity_names_list.size()))) {
		if (entity_names_idx != 0) {
			entities[entity_names_idx - 1]->addChild(entity.getPtr()); //subtract 1 since index 0 is "None"
		}
		else if (entity.hasParent()) {
			entity.getParent()->removeChild(entity.getPtr());
		}
	}


	//----------------------------------------------------------------------------------
	// Draw Component Nodes
	//----------------------------------------------------------------------------------

	// Transform
	if (auto* transform = entity.getComponent<Transform>()) {
		drawComponentNode("Transform", *transform); //multiple transforms are technically allowed but only the first is relevant
	}

	// Model
	if (entity.hasComponent<Model>()) {
		auto models = entity.getAll<Model>();
		for (Model& model : models) {
			std::string name = "Model: " + model.getName();
			drawComponentNode(name.c_str(), model, resource_mgr);
		}
	}

	// Perspective Camera
	if (entity.hasComponent<PerspectiveCamera>()) {
		auto cams = entity.getAll<PerspectiveCamera>();
		for (PerspectiveCamera& cam : cams) {
			drawComponentNode("Perspective Camera", cam);
		}
	}

	// Orthographic Camera
	if (entity.hasComponent<OrthographicCamera>()) {
		auto cams = entity.getAll<OrthographicCamera>();
		for (OrthographicCamera& cam : cams) {
			drawComponentNode("Orthographic Camera", cam);
		}
	}

	// Text
	if (entity.hasComponent<Text>()) {
		auto texts = entity.getAll<Text>();
		for (Text& text : texts) {
			drawComponentNode("Text", text);
		}
	}

	// Ambient Light
	if (entity.hasComponent<AmbientLight>()) {
		auto lights = entity.getAll<AmbientLight>();
		for (AmbientLight& light : lights) {
			drawComponentNode("Ambient Light", light);
		}
	}

	// Directional Light
	if (entity.hasComponent<DirectionalLight>()) {
		auto lights = entity.getAll<DirectionalLight>();
		for (DirectionalLight& light : lights) {
			drawComponentNode("Directional Light", light);
		}
	}

	// Point Light
	if (entity.hasComponent<PointLight>()) {
		auto lights = entity.getAll<PointLight>();
		for (PointLight& light : lights) {
			drawComponentNode("Point Light", light);
		}
	}

	// Spot Light
	if (entity.hasComponent<SpotLight>()) {
		auto lights = entity.getAll<SpotLight>();
		for (SpotLight& light : lights) {
			drawComponentNode("Spot Light", light);
		}
	}

	// Camera Movement
	if (entity.hasComponent<CameraMovement>()) {
		auto components = entity.getAll<CameraMovement>();
		for (CameraMovement& comp : components) {
			drawComponentNode("Camera Movement", comp);
		}
	}

	// Mouse Rotation
	if (entity.hasComponent<MouseRotation>()) {
		auto components = entity.getAll<MouseRotation>();
		for (MouseRotation& comp : components) {
			drawComponentNode("Mouse Rotation", comp);
		}
	}

	// Axis Rotation
	if (entity.hasComponent<AxisRotation>()) {
		auto components = entity.getAll<AxisRotation>();
		for (AxisRotation& comp : components) {
			drawComponentNode("Axis Rotation", comp);
		}
	}

	// Axis Orbit
	if (entity.hasComponent<AxisOrbit>()) {
		auto components = entity.getAll<AxisOrbit>();
		for (AxisOrbit& comp : components) {
			drawComponentNode("Axis Orbit", comp);
		}
	}

	ImGui::End(); //"Properties"
}


template<typename T, typename... ArgsT>
void EntityDetailsWindow::drawComponentNode(gsl::czstring<> text, T& component, ArgsT&&... args) {

	bool dont_delete = true;
	ImGui::PushID(&component);
	if (ImGui::CollapsingHeader(text, &dont_delete)) {
		drawDetails(component, std::forward<ArgsT>(args)...);
	}
	ImGui::PopID();
	if (!dont_delete) {
		component.getOwner()->removeComponent(component);
	}
}


void EntityDetailsWindow::drawAddComponentMenu(Engine& engine, EntityPtr entity_ptr) {

	bool valid_entity = entity_ptr.valid();

	auto& scene         = engine.getScene();
	auto& rendering_mgr = engine.getRenderingMgr();
	auto& device        = rendering_mgr.getDevice();
	auto& resource_mgr  = rendering_mgr.getResourceMgr();

	if (ImGui::BeginMenu("Add Component")) {

		if (ImGui::MenuItem("Orthographic Camera", nullptr, nullptr, valid_entity)) {
			entity_ptr->addComponent<OrthographicCamera>(device, vec2_u32{480, 480});
		}
		if (ImGui::MenuItem("Perspective Camera", nullptr, nullptr, valid_entity)) {
			entity_ptr->addComponent<PerspectiveCamera>(device, vec2_u32{480, 480});
		}
		if (ImGui::MenuItem("Directional Light", nullptr, nullptr, valid_entity)) {
			entity_ptr->addComponent<DirectionalLight>();
		}
		if (ImGui::MenuItem("Point Light", nullptr, nullptr, valid_entity)) {
			entity_ptr->addComponent<PointLight>();
		}
		if (ImGui::MenuItem("Spot Light", nullptr, nullptr, valid_entity)) {
			entity_ptr->addComponent<SpotLight>();
		}

		if (ImGui::BeginMenu("Model")) {
			if (ImGui::MenuItem("From file")) {
				if (auto file = OpenFileDialog(); fs::exists(file)) {
					ModelConfig<VertexPositionNormalTexture> config;
					auto bp = resource_mgr.getOrCreate<ModelBlueprint>(file, config);
					if (valid_entity)
						scene.importModel(entity_ptr, device, bp);
				}
				else Logger::log(LogLevel::err, "Failed to open file dialog");
			}

			if (ImGui::BeginMenu("Geometric Shape")) {
				new_model_menu.drawMenu();
				ImGui::EndMenu();
			}

			ImGui::EndMenu(); //Model
		}

		ImGui::EndMenu(); //Add Component
	}
}


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


template<>
void EntityDetailsWindow::drawDetails(Transform& transform) {

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
	    RenderMode::Deferred
	};

	auto render_mode = static_cast<int>(settings.getRenderMode());
	if (ImGui::Combo("Render Mode", &render_mode, render_mode_names, static_cast<int>(std::size(render_mode_names))))
		settings.setRenderMode(static_cast<RenderMode>(render_mode));


	//----------------------------------------------------------------------------------
	// Lighting Mode
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring<> light_mode_names[] = {
	    "Default",
	    "BRDF",
	    "FalseColorFullbright",
	    "FalseColorNormal",
	    "FalseColorDepth"
	};
	static constexpr LightingMode light_modes[] = {
	    LightingMode::Default,
	    LightingMode::BRDF,
	    LightingMode::FalseColorFullbright,
	    LightingMode::FalseColorNormal,
	    LightingMode::FalseColorDepth
	};

	auto light_mode = static_cast<int>(settings.getLightingMode());
	if (ImGui::Combo("Lighting Mode", &light_mode, light_mode_names, static_cast<int>(std::size(light_mode_names))))
		settings.setLightingMode(static_cast<LightingMode>(light_mode));


	//----------------------------------------------------------------------------------
	// BRDF
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring<> brdf_names[] = {
	    "Lambert",
	    "Blinn-Phong",
	    "Cook-Torrance"
	};
	static constexpr BRDF brdfs[] = {
	    BRDF::Lambert,
	    BRDF::BlinnPhong,
	    BRDF::CookTorrance
	};

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


template<>
void EntityDetailsWindow::drawDetails(PerspectiveCamera& camera) {

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
	static constexpr u32 v_min_pos  = 0;
	static constexpr u32 v_max      = 15360;

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


template<>
void EntityDetailsWindow::drawDetails(OrthographicCamera& camera) {

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
	static constexpr u32 v_min_pos  = 0;
	static constexpr u32 v_max      = 15360;

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


template<>
void EntityDetailsWindow::drawDetails(CameraMovement& movement) {

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


template<>
void EntityDetailsWindow::drawDetails(MouseRotation& rotation) {

	DrawComponentState(rotation);

	f32 sensitivity = rotation.getSensitivity();

	if (ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f, 0.0f, FLT_MAX)) {
		rotation.setSensitivity(sensitivity);
	}
}


template<>
void EntityDetailsWindow::drawDetails(Text& text) {

	DrawComponentState(text);

	std::string str = WstrToStr(text.getText());

	if (ImGui::InputTextMultiline("Text", &str)) {
		text.setText(StrToWstr(str));
	}
}


template<>
void EntityDetailsWindow::drawDetails(Model& model, ResourceMgr& resource_mgr) {

	DrawComponentState(model);

	ImGui::Separator();

	auto& mat = model.getMaterial();
	ImGui::Text("Material: %s", mat.name.c_str());
	ImGui::Spacing();

	//----------------------------------------------------------------------------------
	// Material properties
	//----------------------------------------------------------------------------------
	ImGui::ColorEdit4("Base Color", mat.params.base_color.data());
	ImGui::DragFloat("Metalness", &mat.params.metalness, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Roughness", &mat.params.roughness, 0.01f, 0.0f, 1.0f);
	ImGui::ColorEdit3("Emissive", mat.params.emissive.data());

	ImGui::Spacing();

	//----------------------------------------------------------------------------------
	// Texture
	//----------------------------------------------------------------------------------

	auto& maps = mat.maps;

	const char* diffuse_preview   = mat.maps.base_color      ? maps.base_color->getGUID().c_str()      : "None";
	const char* normal_preview    = mat.maps.normal          ? maps.normal->getGUID().c_str()          : "None";
	const char* matparams_preview = mat.maps.material_params ? maps.material_params->getGUID().c_str() : "None";
	const char* emissive_preview  = maps.emissive            ? mat.maps.emissive->getGUID().c_str()    : "None";

	drawResourceMapComboBox<Texture>("Base Color Map",      diffuse_preview,   resource_mgr, maps.base_color);
	drawResourceMapComboBox<Texture>("Normal Map",          normal_preview,    resource_mgr, maps.normal);
	drawResourceMapComboBox<Texture>("Material Params Map", matparams_preview, resource_mgr, maps.material_params);
	drawResourceMapComboBox<Texture>("Emissive Map",        emissive_preview,  resource_mgr, maps.emissive);

	ImGui::Spacing();

	//----------------------------------------------------------------------------------
	// Material shader
	//----------------------------------------------------------------------------------
	const char* shader_title_preview = mat.shader ? mat.shader->getGUID().c_str() : "Default";

	drawResourceMapComboBox<PixelShader>("Shader", shader_title_preview, resource_mgr, mat.shader);

	ImGui::Separator();

	//----------------------------------------------------------------------------------
	// Shadows
	//----------------------------------------------------------------------------------
	bool shadows = model.castsShadows();
	if (ImGui::Checkbox("Casts Shadows", &shadows))
		model.setShadows(shadows);
}


template<typename ResourceT>
void EntityDetailsWindow::drawResourceMapComboBox(const char* name,
                                                  const char* preview_text,
                                                  const ResourceMgr& resource_mgr,
                                                  std::shared_ptr<ResourceT>& selection_receiver) {
	if (ImGui::BeginCombo(name, preview_text)) {
		const auto& resource_map = resource_mgr.getResourceMap<ResourceT>();
		res_map_names.clear();

		if (ImGui::Selectable("None")) {
			selection_receiver = nullptr;
		}
		for (const auto& [key, sharedptr] : resource_map) {
			if (!sharedptr) continue;

			res_map_names.push_back(std::ref(sharedptr->getGUID()));

			if constexpr (std::is_same_v<Texture, ResourceT>) {
				ImGui::Image(sharedptr->get(), {15, 15});
				ImGui::SameLine();
			}
			if (ImGui::Selectable(res_map_names.back().get().c_str())) {
				selection_receiver = sharedptr;
			}
		}
		ImGui::EndCombo();
	}
}


template<>
void EntityDetailsWindow::drawDetails(AmbientLight& light) {

	DrawComponentState(light);

	auto color = light.getColor();
	if (ImGui::ColorEdit3("Ambient Color", color.data()))
		light.setColor(color);
}


template<>
void EntityDetailsWindow::drawDetails(DirectionalLight& light) {

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


template<>
void EntityDetailsWindow::drawDetails(PointLight& light) {

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


template<>
void EntityDetailsWindow::drawDetails(SpotLight& light) {

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


template<>
void EntityDetailsWindow::drawDetails(AxisRotation& rotation) {

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


template<>
void EntityDetailsWindow::drawDetails(AxisOrbit& orbit) {

	DrawComponentState(orbit);

	auto axis = XMStore<vec3_f32>(orbit.getAxis());
	if (ImGui::InputFloat3("Axis", axis.data())) {
		orbit.setAxis(axis);
	}
}

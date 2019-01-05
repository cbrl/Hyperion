#include "user_interface.h"
#include "engine/engine.h"

#include "entities/entities.h"
#include "components/components.h"

#include "imgui.h"
#include "misc/stl/imgui_stl.h"
#include "imgui_addons/ImGuizmo/ImGuizmo.h"
#include "imgui_addons/metrics_gui/metrics_gui/metrics_gui.h"

#include "resource/resource_mgr.h"
#include "resource/model/blueprint_factory.h"
#include "scene/scene.h"
#include "log/log.h"
#include "os/windows/win_utils.h"


template<typename T>
struct SelectableTree final {
public:
	template<typename... ArgsT>
	bool newNode(T* ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_OpenOnArrow
		                   | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	template <typename... ArgsT>
	bool newLeafNode(T* ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_Leaf
		                   | ImGuiTreeNodeFlags_Bullet
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	void endNode() {
		ImGui::TreePop();
	}

	T* getSelected() const noexcept {
		return selected;
	}

	bool isSelected(T* ptr) const noexcept {
		return selected == ptr;
	}

private:
	template <typename... ArgsT>
	bool node(T* ptr, ImGuiTreeNodeFlags flags, gsl::czstring<> fmt, ArgsT... args) {
		const bool is_open = ImGui::TreeNodeEx(ptr, flags, fmt, std::forward<ArgsT>(args)...);
		if (ImGui::IsItemClicked()) {
			selected = ptr;
		}
		return is_open;
	}


private:
	T* selected = nullptr;
};


template <>
struct SelectableTree<EntityPtr> final {
public:
	template <typename... ArgsT>
	bool newNode(const EntityPtr& ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_OpenOnArrow
		                   | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	template <typename... ArgsT>
	bool newLeafNode(const EntityPtr& ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_Leaf
		                   | ImGuiTreeNodeFlags_Bullet
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	void endNode() {
		ImGui::TreePop();
	}

	EntityPtr getSelected() const noexcept {
		return selected;
	}

	bool isSelected(const EntityPtr& ptr) const noexcept {
		return selected == ptr;
	}

private:
	template <typename... ArgsT>
	bool node(const EntityPtr& ptr, ImGuiTreeNodeFlags flags, gsl::czstring<> fmt, ArgsT... args) {
		const bool is_open = ImGui::TreeNodeEx(ptr.get(), flags, fmt, std::forward<ArgsT>(args)...);
		if (ImGui::IsItemClicked()) {
			selected = ptr;
		}
		return is_open;
	}


private:
	EntityPtr selected = {};
};


//----------------------------------------------------------------------------------
// Scene Tree global
//----------------------------------------------------------------------------------
SelectableTree<EntityPtr> g_scene_tree;


//----------------------------------------------------------------------------------
// Enum for "Add Model" popups
//----------------------------------------------------------------------------------
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
void DrawComponentState(T& component, gsl::czstring<> name = "") {
	ImGui::Text(name);
	ImGui::SameLine();
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
		"Deferred"
	};
	static constexpr RenderMode render_modes[] = {
		RenderMode::Forward,
		RenderMode::ForwardPlus,
		RenderMode::Deferred
	};
	static_assert(std::size(render_mode_names) == std::size(render_modes));

	auto render_mode = static_cast<int>(settings.getRenderMode());
	if (ImGui::Combo("Render Mode", &render_mode, render_mode_names, static_cast<int>(std::size(render_mode_names))))
		settings.setRenderMode(static_cast<RenderMode>(render_mode));


	//----------------------------------------------------------------------------------
	// Lighting Mode
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring<> light_mode_names[] = {
		"Default",
		"Unlint",
		"FalseColorNormal",
		"FalseColorDepth"
	};
	static constexpr LightingMode light_modes[] = {
		LightingMode::Default,
		LightingMode::Unlit,
		LightingMode::FalseColorNormal,
		LightingMode::FalseColorDepth
	};
	static_assert(std::size(light_mode_names) == std::size(light_modes));

	auto light_mode = static_cast<int>(settings.getLightingMode());
	if (ImGui::Combo("Lighting Mode", &light_mode, light_mode_names, static_cast<int>(std::size(light_mode_names))))
		settings.setLightingMode(static_cast<LightingMode>(light_mode));


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
	ImGui::ColorEdit3("Fog Color", fog.color.data());
	ImGui::DragFloat("Fog Start", &fog.start, 0.1f);
	ImGui::DragFloat("Fog Range", &fog.range, 0.1f);
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

	DrawComponentState(model, model.getName().c_str());

	bool shadows = model.castsShadows();
	if (ImGui::Checkbox("Casts Shadows", &shadows))
		model.setShadows(shadows);

	// Material properties
	auto& mat = model.getMaterial();
	ImGui::Text("Material: %s", mat.name.c_str());

	ImGui::ColorEdit4("Base Color", mat.params.base_color.data());
	ImGui::DragFloat("Metalness", &mat.params.metalness, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Roughness", &mat.params.roughness, 0.01f, 0.0f, 1.0f);
	//TODO: ImGui::ColorEdit3("Emissive", mat.params.emissive.data());
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

	if (ImGui::TreeNodeEx(&node, MakeTreeNodeFlags(), node.getName().c_str())) {

		DrawDetails(node);

		node.forEachModel([](Model& model) {
			if (ImGui::TreeNodeEx(&model, MakeTreeNodeFlags(), model.getName().c_str())) {
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

	auto base_color = light.getBaseColor();
	if (ImGui::ColorEdit3("Base Color", base_color.data()))
		light.setBaseColor(base_color);

	auto intensity = light.getIntensity();
	if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, FLT_MAX))
		light.setIntensity(intensity);

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

	auto base_color = light.getBaseColor();
	if (ImGui::ColorEdit3("Base Color", base_color.data()))
		light.setBaseColor(base_color);

	auto intensity = light.getIntensity();
	if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, FLT_MAX))
		light.setIntensity(intensity);

	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, FLT_MAX))
		light.setRange(range);

	auto attenuation = light.getAttenuation();
	if (ImGui::DragFloat3("Attenuation", attenuation.data(), 0.01f, 0.0f, 1.0f))
		light.setAttenuation(attenuation);

	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void DrawDetails(SpotLight& light) {

	DrawComponentState(light);

	auto base_color = light.getBaseColor();
	if (ImGui::ColorEdit3("Base Color", base_color.data()))
		light.setBaseColor(base_color);

	auto intensity = light.getIntensity();
	if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, FLT_MAX))
		light.setIntensity(intensity);

	auto range = light.getRange();
	if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, FLT_MAX))
		light.setRange(range);

	auto attenuation = light.getAttenuation();
	if (ImGui::DragFloat3("Attenuation", attenuation.data(), 0.01f, 0.0f, 1.0f))
		light.setAttenuation(attenuation);

	auto umbra = light.getUmbraAngle();
	if (ImGui::DragFloat("Umbra", &umbra, 0.01f, 0.01f, XM_PIDIV2))
		light.setUmbraAngle(umbra);

	auto penumbra = light.getPenumbraAngle();
	if (ImGui::DragFloat("Penumbra", &penumbra, 0.01f, 0.01f, XM_PIDIV2))
		light.setPenumbraAngle(penumbra);

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
		if (x_enable) rotation.addAxis(AxisRotation::Axis::X);
		else rotation.removeAxis(AxisRotation::Axis::X);
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
		if (y_enable) rotation.addAxis(AxisRotation::Axis::Y);
		else rotation.removeAxis(AxisRotation::Axis::Y);
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

	auto axis = XMStore<vec3_f32>(orbit.getAxis());
	if (ImGui::InputFloat3("Axis", axis.data())) {
		orbit.setAxis(axis);
	}
}




//----------------------------------------------------------------------------------
//
//   Entity Details
//
//----------------------------------------------------------------------------------

// Draw a node in the tree, and its details if selected
template <typename T>
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


// Draw the details panel for the specified entity
void DrawEntityDetails(Entity& entity, Scene& scene) {

	ImGui::Begin("Properties");

	const auto& entities = scene.getEntities();

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
		}
		else if (entity.hasParent()) {
			entity.getParent()->removeChild(entity.getPtr());
		}
	}


	//----------------------------------------------------------------------------------
	// Draw Component Nodes
	//----------------------------------------------------------------------------------


	// Transform
	if (entity.hasComponent<Transform>()) {
		auto transforms = entity.getAll<Transform>();
		for (auto* transform : transforms) {
			DrawComponentNode("Transform", *transform);
		}
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
void DrawEntityNode(EntityPtr entity_ptr, Scene& scene) {

	if (!entity_ptr.valid())
		return;

	auto* entity = entity_ptr.get();
	auto  handle = entity_ptr.getHandle();

	//----------------------------------------------------------------------------------
	// Draw Entity Node
	//----------------------------------------------------------------------------------
	bool node_open = true;
	if (entity->hasChildren()) {
		node_open = g_scene_tree.newNode(entity_ptr, entity->getName().c_str());
	}
	else {
		node_open = g_scene_tree.newLeafNode(entity_ptr, entity->getName().c_str());
	}

	if (node_open) {
		// Draw any child entities in this node
		if (entity->hasChildren()) {
			entity->forEachChildRecursive([&scene](EntityPtr& child) {
				DrawEntityNode(child, scene);
			});
		}

		g_scene_tree.endNode();
	}
}


// Draw the scene tree
void DrawTree(Scene& scene) {

	if (ImGui::BeginChild("object list", ImVec2(250, 0))) {

		ImGui::Text("%s (Entities: %llu)", scene.getName().c_str(), scene.getEntities().size());
		ImGui::Separator();

		// Draw a tree node for each root entity
		auto& entities = scene.getEntities();
		for (const auto& entity_ptr : entities) {
			if (entity_ptr->hasParent()) continue;
			DrawEntityNode(entity_ptr, scene);
		}

		if (g_scene_tree.getSelected()) {
			DrawEntityDetails(*g_scene_tree.getSelected(), scene);
		}
	}

	ImGui::EndChild();
}




//----------------------------------------------------------------------------------
//
//   "Add Model" Popup Windows
//
//----------------------------------------------------------------------------------

void ProcNewModelPopups(ID3D11Device& device,
                        Scene& scene,
                        ResourceMgr& resource_mgr,
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
		static f32  size         = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateCube(resource_mgr, config, size);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Box", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static vec3_f32 size         = { 1.0f, 1.0f, 1.0f };
		static bool     flip_winding = false;

		ImGui::InputFloat3("Size", size.data());
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateBox(resource_mgr, config, size);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
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
		static bool   flip_winding = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 64) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateSphere(resource_mgr, config, diameter, tessellation);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
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
		static bool   flip_winding = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 6) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is extremely high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateGeoSphere(resource_mgr, config, diameter, tessellation);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
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
		static bool   flip_winding = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCylinder(resource_mgr, config, diameter, height, tessellation);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
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
		static bool   flip_winding = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCone(resource_mgr, config, diameter, height, tessellation);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Torus", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32    thickness    = 0.333f;
		static f32    diameter     = 1.0f;
		static size_t tessellation = 32;
		static bool   flip_winding = false;

		ImGui::InputFloat("Thickness", &thickness);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateTorus(resource_mgr, config, diameter, thickness, tessellation);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Tetrahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size         = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateTetrahedron(resource_mgr, config, size);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Octahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size         = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateOctahedron(resource_mgr, config, size);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Dodecahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size         = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateDodecahedron(resource_mgr, config, size);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Icosahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  size         = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateIcosahedron(resource_mgr, config, size);
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<ModelRoot>(device, bp);
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
//   Scene Menu
//
//----------------------------------------------------------------------------------

void DrawAddComponentMenu(ID3D11Device& device,
                          ResourceMgr& resource_mgr,
                          Scene& scene,
                          ModelType& add_model_popup) {
	
	if (ImGui::BeginMenu("Add Component")) {

		if (ImGui::MenuItem("Orthographic Camera")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<OrthographicCamera>(device, vec2_u32{ 480, 480 });
		}
		if (ImGui::MenuItem("Perspective Camera")) {
			if (auto entity = g_scene_tree.getSelected())
				entity->addComponent<PerspectiveCamera>(device, vec2_u32{ 480, 480 });
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
				}
				else Logger::log(LogLevel::err, "Failed to open file dialog");
			}

			if (ImGui::BeginMenu("Geometric Shape")) {
				if (ImGui::MenuItem("Cube"))         add_model_popup = ModelType::Cube;
				if (ImGui::MenuItem("Box"))          add_model_popup = ModelType::Box;
				if (ImGui::MenuItem("Sphere"))       add_model_popup = ModelType::Sphere;
				if (ImGui::MenuItem("GeoSphere"))    add_model_popup = ModelType::GeoSphere;
				if (ImGui::MenuItem("Cylinder"))     add_model_popup = ModelType::Cylinder;
				if (ImGui::MenuItem("Cone"))         add_model_popup = ModelType::Cone;
				if (ImGui::MenuItem("Torus"))        add_model_popup = ModelType::Torus;
				if (ImGui::MenuItem("Tetrahedron"))  add_model_popup = ModelType::Tetrahedron;
				if (ImGui::MenuItem("Octahedron"))   add_model_popup = ModelType::Octahedron;
				if (ImGui::MenuItem("Dodecahedron")) add_model_popup = ModelType::Dodecahedron;
				if (ImGui::MenuItem("Icosahedron"))  add_model_popup = ModelType::Icosahedron;

				ImGui::EndMenu(); //Geometric Shape
			}

			ImGui::EndMenu(); //Model
		}

		ImGui::EndMenu(); //Add Component
	}
}


void DrawEntityMenu(ID3D11Device& device,
                    ResourceMgr& resource_mgr,
                    Scene& scene,
                    ModelType& add_model_popup) {

	if (ImGui::BeginMenu("Entity")) {

		if (ImGui::MenuItem("New")) {
			scene.addEntity();
		}
		
		if (ImGui::BeginMenu("Selected", g_scene_tree.getSelected().valid())) {
			DrawAddComponentMenu(device, resource_mgr, scene , add_model_popup);
			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) {
				scene.removeEntity(g_scene_tree.getSelected()->getPtr());
			}

			ImGui::EndMenu(); //Selected
		}

		ImGui::EndMenu(); //Entity
	}
}


void DrawSceneMenu(ID3D11Device& device,
                   ResourceMgr& resource_mgr,
                   Scene& scene,
                   ModelType& add_model_popup) {

	if (ImGui::BeginMenuBar()) {
		DrawEntityMenu(device, resource_mgr, scene, add_model_popup);
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
				const auto refresh       = static_cast<u32>(round(exact_refresh));
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

	static MetricsGuiPlot frame_plot;
	static MetricsGuiMetric frame_time("Frame Time", "s", MetricsGuiMetric::USE_SI_UNIT_PREFIX);
	static MetricsGuiMetric fps("FPS", "", 0);

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
	}

	const auto& timer = engine.getTimer();

	frame_time.AddNewValue(static_cast<float>(timer.deltaTime()));
	fps.AddNewValue(1.0f / timer.deltaTime());
	frame_plot.UpdateAxes();

	if (ImGui::Begin("Metrics")) {
		frame_plot.DrawList();
		ImGui::End();
	}
}




//----------------------------------------------------------------------------------
//
//   UserInterface
//
//----------------------------------------------------------------------------------

UserInterface::UserInterface(ID3D11Device& device, ResourceMgr& resource_mgr)
    : device(device)
    , resource_mgr(resource_mgr) {

}


void UserInterface::update(Engine& engine)  {

	//ImGui::ShowDemoWindow();

	auto& scene = engine.getScene();

	// Setup window layout
	ImGui::SetNextWindowSize(ImVec2(275, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(375, 425), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties");
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

		ModelType add_model_popup = ModelType::None;

		// Draw menu
		DrawSceneMenu(device, resource_mgr, scene, add_model_popup);

		// Process model popup
		ProcNewModelPopups(device, scene, resource_mgr, add_model_popup);

		// Draw tree
		DrawTree(scene);
	}
	ImGui::End();
}
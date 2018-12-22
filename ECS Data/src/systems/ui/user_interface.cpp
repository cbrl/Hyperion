#include "user_interface.h"
#include "engine/engine.h"

#include "entities/entities.h"
#include "components/components.h"

#include "imgui.h"
#include "misc/stl/imgui_stl.h"

#include "resource/resource_mgr.h"
#include "resource/model/blueprint_factory.h"
#include "scene/scene.h"
#include "log/log.h"
#include "os/windows/win_utils.h"


//----------------------------------------------------------------------------------
// Selected Item
//----------------------------------------------------------------------------------
void*       g_selected = nullptr;
IComponent* g_selected_component = nullptr;
EntityPtr   g_selected_entity = {};

void SetSelected(void* item) {
	g_selected = item;
	g_selected_component = nullptr;
	g_selected_entity = EntityPtr{};
}
void SetSelected(IComponent* component) {
	g_selected = nullptr;
	g_selected_component = component;
	g_selected_entity = EntityPtr{};
}
void SetSelected(const EntityPtr& entity) {
	g_selected = nullptr;
	g_selected_component = nullptr;
	g_selected_entity = entity;
}
bool IsSelected(void* item) {
	return g_selected == item || g_selected_component == item;
}
bool IsSelected(const EntityPtr& entity) {
	return g_selected_entity == entity;
}


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

ImGuiTreeNodeFlags MakeTreeNodeFlags(void* compare_selected) {
	return ImGuiTreeNodeFlags_OpenOnArrow
	       | ImGuiTreeNodeFlags_OpenOnDoubleClick
	       | (IsSelected(compare_selected) ? ImGuiTreeNodeFlags_Selected : 0);
}

ImGuiTreeNodeFlags MakeTreeNodeFlags(const EntityPtr& compare_selected) {
	return ImGuiTreeNodeFlags_OpenOnArrow
	       | ImGuiTreeNodeFlags_OpenOnDoubleClick
	       | (IsSelected(compare_selected) ? ImGuiTreeNodeFlags_Selected : 0);
}

ImGuiTreeNodeFlags MakeTreeLeafFlags(void* compare_selected) {
	return ImGuiTreeNodeFlags_Leaf
	       | ImGuiTreeNodeFlags_Bullet
	       | (IsSelected(compare_selected) ? ImGuiTreeNodeFlags_Selected : 0);
}



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


void DrawDetails(Entity& entity, const std::vector<EntityPtr>& entities) {

	ImGui::Text("Entity");
	ImGui::Text("Index: %d", entity.getPtr().getHandle().index);
	ImGui::Text("Counter: %d", entity.getPtr().getHandle().counter);
	ImGui::Separator();


	//----------------------------------------------------------------------------------
	// Name
	//----------------------------------------------------------------------------------
	ImGui::InputText("Name", &entity.getName());


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
			entities[index-1]->addChild(entity.getPtr()); //subtract 1 since index 0 is "None"
		}
		else {
			entity.getParent()->removeChild(entity.getPtr());
		}
	}
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

	ImGui::Text("Camera");
	ImGui::Separator();

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

	ImGui::Text("Camera");
	ImGui::Separator();
	
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


void DrawDetails(Text& text) {

	DrawComponentState(text);

	std::string str = WstrToStr(text.getText());

	if (ImGui::InputTextMultiline("Text", &str)) {
		text.setText(StrToWstr(str));
	}
}


void DrawDetails(ModelRoot& root) {
	DrawComponentState(root);
}
void DrawDetails(ModelNode& node) {
	ImGui::Text("State");
	ImGui::Separator();
	if (ImGui::Button("Active"))
		node.setModelsActive(true);
	ImGui::SameLine();
	if (ImGui::Button("Inactive"))
		node.setModelsActive(false);
}
void DrawDetails(Model& model) {

	DrawComponentState(model);

	ImGui::Text(model.getName().c_str());
	ImGui::Separator();

	bool shadows = model.castsShadows();
	if (ImGui::Checkbox("Casts Shadows", &shadows))
		model.setShadows(shadows);

	// Material properties
	auto& mat = model.getMaterial();
	std::string name = "Material: " + mat.name;
	ImGui::Text(name.c_str());

	ImGui::ColorEdit3("Diffuse Color", mat.params.diffuse.data());
	ImGui::ColorEdit3("Ambient Color", mat.params.ambient.data());
	ImGui::ColorEdit3("Specular Color", mat.params.specular.data());
	ImGui::DragFloat("Specular Scale", &mat.params.spec_scale, 0.01f, 0.0f, FLT_MAX);
	ImGui::DragFloat("Specular Exponent", &mat.params.spec_exponent, 0.01f, 0.0f, FLT_MAX);
	ImGui::DragFloat("Opacity", &mat.params.opacity, 0.01f, 0.0f, 1.0f);
	//TODO: ImGui::DragFloat("Index of Refraction", &mat.params.refraction_index, 0.01f, 0.0f, FLT_MAX);
	TODO: ImGui::Checkbox("Reflections", &mat.params.reflection);
}


void DrawDetails(AmbientLight& light) {

	DrawComponentState(light);

	ImGui::Text("Ambient Light");
	ImGui::Separator();

	auto color = light.getColor();
	if (ImGui::ColorEdit3("Ambient Color", color.data()))
		light.setColor(color);
}


void DrawDetails(DirectionalLight& light) {

	DrawComponentState(light);

	ImGui::Text("Directional Light");
	ImGui::Separator();

	auto diffuse = light.getDiffuseColor();
	if (ImGui::ColorEdit3("Diffuse Color", diffuse.data()))
		light.setDiffuseColor(diffuse);

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

// Draw the details panel for a component
template<typename T, typename... ArgsT>
void DrawDetailsPanel(T& item, ArgsT&... args) {
	ImGui::Begin("Properties");
	DrawDetails(item, std::forward<ArgsT>(args)...);
	ImGui::End();
}

// Draw a leaf node in the tree, and its details if selected
template<typename T>
void DrawLeafNode(gsl::czstring<> text, T& item) {

	ImGui::TreeNodeEx(&item, MakeTreeLeafFlags(&item), text);

	if (IsSelected(&item)) {
		DrawDetailsPanel(item);
	}
	if (ImGui::IsItemClicked()) {
		SetSelected(&item);
	}

	ImGui::TreePop();
}


// Draw the model hierarchy
void DrawModelNodes(ModelNode& node) {

	const bool node_open = ImGui::TreeNodeEx(&node, MakeTreeNodeFlags(&node), node.getName().c_str());

	if (IsSelected(&node)) {
		DrawDetailsPanel(node);
	}
	if (ImGui::IsItemClicked()) {
		SetSelected(&node);
	}

	if (!node_open) return;

	node.forEachModel([](Model& model) {
		DrawLeafNode(model.getName().c_str(), model);
	});
	node.forEachNode([](ModelNode& node) {
		DrawModelNodes(node);
	});

	ImGui::TreePop();
}

// Draw the model component node, and the model hierarchy if the node is open
void DrawModelTree(ModelRoot& root) {

	std::string name = "ModelRoot: " + root.getName();
	const bool node_open = ImGui::TreeNodeEx(&root, MakeTreeNodeFlags(&root), name.c_str());

	if (IsSelected(&root)) {
		DrawDetailsPanel(root);
	}
	if (ImGui::IsItemClicked()) {
		SetSelected(&root);
	}

	if (!node_open) return;
	DrawModelNodes(root.getRootNode());
	ImGui::TreePop();
}


// Draw a single tree node
void DrawEntityNode(EntityPtr entity_ptr, Scene& scene) {

	if (!entity_ptr.valid())
		return;

	auto* entity = entity_ptr.get();
	auto  handle = entity_ptr.getHandle();

	//----------------------------------------------------------------------------------
	// Draw Entity Node
	//----------------------------------------------------------------------------------
	const bool node_open = ImGui::TreeNodeEx(entity, MakeTreeNodeFlags(entity_ptr), entity->getName().c_str());

	if (ImGui::IsItemClicked()) {
		SetSelected(entity_ptr);
	}

	if (IsSelected(entity_ptr)) {
		DrawDetailsPanel(*entity, scene.getEntities());
	}

	// Return early if the node isn't open
	if (!node_open) return;


	//----------------------------------------------------------------------------------
	// Draw Component Nodes
	//----------------------------------------------------------------------------------

	// Transform
	if (auto* transform = entity->getComponent<Transform>()) {
		DrawLeafNode("Transform", *transform);
	}

	// Perspective Camera
	if (entity->hasComponent<PerspectiveCamera>()) {
		auto cams = entity->getAll<PerspectiveCamera>();
		for (auto* cam : cams) {
			DrawLeafNode("Perspective Camera", *cam);
		}
	}

	// Orthographic Camera
	if (entity->hasComponent<OrthographicCamera>()) {
		auto cams = entity->getAll<OrthographicCamera>();
		for (auto* cam : cams) {
			DrawLeafNode("Orthographic Camera", *cam);
		}
	}

	// Text
	if (entity->hasComponent<Text>()) {
		auto texts = entity->getAll<Text>();
		for (auto* text : texts) {
			DrawLeafNode("Text", *text);
		}
	}

	// Model Root
	if (entity->hasComponent<ModelRoot>()) {
		auto roots = entity->getAll<ModelRoot>();
		for (auto* root : roots) {
			DrawModelTree(*root);
		}
	}

	// Ambient Light
	if (entity->hasComponent<AmbientLight>()) {
		auto lights = entity->getAll<AmbientLight>();
		for (auto* light : lights) {
			DrawLeafNode("Ambient Light", *light);
		}
	}

	// Directional Light
	if (entity->hasComponent<DirectionalLight>()) {
		auto lights = entity->getAll<DirectionalLight>();
		for (auto* light : lights) {
			DrawLeafNode("Directional Light", *light);
		}
	}

	// Point Light
	if (entity->hasComponent<PointLight>()) {
		auto lights = entity->getAll<PointLight>();
		for (auto* light : lights) {
			DrawLeafNode("Point Light", *light);
		}
	}

	// Spot Light
	if (entity->hasComponent<SpotLight>()) {
		auto lights = entity->getAll<SpotLight>();
		for (auto* light : lights) {
			DrawLeafNode("Spot Light", *light);
		}
	}

	// Camera Movement
	if (entity->hasComponent<CameraMovement>()) {
		auto components = entity->getAll<CameraMovement>();
		for (auto* comp : components) {
			DrawLeafNode("Camera Movement", *comp);
		}
	}

	// Mouse Rotation
	if (entity->hasComponent<MouseRotation>()) {
		auto components = entity->getAll<MouseRotation>();
		for (auto* comp : components) {
			DrawLeafNode("Mouse Rotation", *comp);
		}
	}

	// Axis Rotation
	if (entity->hasComponent<AxisRotation>()) {
		auto components = entity->getAll<AxisRotation>();
		for (auto* comp : components) {
			DrawLeafNode("Axis Rotation", *comp);
		}
	}

	// Axis Orbit
	if (entity->hasComponent<AxisOrbit>()) {
		auto components = entity->getAll<AxisOrbit>();
		for (auto* comp : components) {
			DrawLeafNode("Axis Orbit", *comp);
		}
	}


	// Draw any child entities in this node
	if (entity->hasChildren()) {
		entity->forEachChildRecursive([&scene](EntityPtr& child) {
			DrawEntityNode(child, scene);
		});
	}

	ImGui::TreePop();
}


// Draw a tree node for each root entity
void DrawTreeNodes(Scene& scene) {

	auto& entities = scene.getEntities();

	for (const auto& entity_ptr : entities) {

		// Skip the entity if it's a child
		if (entity_ptr->hasParent()) continue;

		DrawEntityNode(entity_ptr, scene);
	}
}


// Draw the scene tree
void DrawTree(Scene& scene) {

	if (ImGui::BeginChild("object list", ImVec2(250, 0))) {
		const bool node_open = ImGui::TreeNodeEx(&scene, MakeTreeNodeFlags(&scene), scene.getName().c_str());

		if (ImGui::IsItemClicked())
			SetSelected(&scene);

		if (node_open) {
			DrawTreeNodes(scene);
			ImGui::TreePop();
		}

		if (IsSelected(&scene)) {
			DrawDetailsPanel(scene);
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
                        EntityPtr entity,
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			auto bp = BlueprintFactory::CreateCube(resource_mgr, config, size, invertn);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			auto bp = BlueprintFactory::CreateBox(resource_mgr, config, size, invertn);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateSphere(resource_mgr, config, diameter, tessellation, invertn);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateGeoSphere(resource_mgr, config, diameter, tessellation);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCylinder(resource_mgr, config, diameter, height, tessellation);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCone(resource_mgr, config, diameter, height, tessellation);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Torus", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32  thickness      = 0.333f;
		static f32  diameter       = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords     = false;

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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateTorus(resource_mgr, config, diameter, thickness, tessellation);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			auto bp = BlueprintFactory::CreateTetrahedron(resource_mgr, config, size);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			auto bp = BlueprintFactory::CreateOctahedron(resource_mgr, config, size);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			auto bp = BlueprintFactory::CreateDodecahedron(resource_mgr, config, size);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = rhcoords;
			auto bp = BlueprintFactory::CreateIcosahedron(resource_mgr, config, size);
			g_selected_entity->addComponent<ModelRoot>(device, bp);
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
			g_selected_entity->addComponent<OrthographicCamera>(device, vec2_u32{ 480, 480 });
		}
		if (ImGui::MenuItem("Perspective Camera")) {
			g_selected_entity->addComponent<PerspectiveCamera>(device, vec2_u32{ 480, 480 });
		}
		if (ImGui::MenuItem("Directional Light")) {
			g_selected_entity->addComponent<DirectionalLight>();
		}
		if (ImGui::MenuItem("Point Light")) {
			g_selected_entity->addComponent<PointLight>();
		}
		if (ImGui::MenuItem("Spot Light")) {
			g_selected_entity->addComponent<SpotLight>();
		}

		if (ImGui::BeginMenu("Model")) {
			if (ImGui::MenuItem("From file")) {
				wchar_t szFile[512] = {};

				if (OpenFilePicker(gsl::not_null<wchar_t*>(szFile), 512)) {
					ModelConfig<VertexPositionNormalTexture> config;
					auto bp = resource_mgr.getOrCreate<ModelBlueprint>(szFile, config);
					g_selected_entity->addComponent<ModelRoot>(device, bp);
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
		
		if (ImGui::BeginMenu("Selected", g_selected_entity.valid())) {
			DrawAddComponentMenu(device, resource_mgr, scene , add_model_popup);
			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) {
				scene.removeEntity(g_selected_entity);
			}

			ImGui::EndMenu(); //Selected
		}

		ImGui::EndMenu(); //Entity
	}

	if (ImGui::BeginMenu("Component")) {
		if (ImGui::MenuItem("Delete Selected", nullptr, nullptr, g_selected_component != nullptr)) {
			g_selected_component->getOwner()->removeComponent(g_selected_component);
		}
		ImGui::EndMenu();
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
//   FPS Display
//
//----------------------------------------------------------------------------------
void DrawFPSDisplay(Engine& engine) {

	const auto& timer = engine.getTimer();
	const auto& fps_counter = engine.getFPSCounter();

	static int vec_size = 30;
	static std::vector<float> fps_history(vec_size, 0.0f);

	static f32 fps = 0.0f;
	static f64 dt  = 0.0;

	dt += timer.deltaTime();
	if (dt >= fps_counter.getWaitTime().count()) {
		fps = fps_counter.getFPS();
		fps_history.push_back(fps);
		dt = 0.0;
	}
	if (fps_history.size() > vec_size) {
		fps_history.erase(fps_history.begin(), fps_history.begin() + (fps_history.size() - vec_size));
	}

	if (ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::BeginChild("FPS Histogram", {250, 50});
		ImGui::PlotLines("", fps_history.data(), static_cast<int>(fps_history.size()), 0, nullptr, 0, FLT_MAX, {250, 50});
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("FPS Stats", {150, 50});
		ImGui::Text("FPS: %.2f", fps);
		ImGui::Text("Frame Time: %.2fms", timer.deltaTime());
		ImGui::EndChild();
	}
	ImGui::End();
}




//----------------------------------------------------------------------------------
//
//   Update
//
//----------------------------------------------------------------------------------
void UserInterface::update(Engine& engine)  {

	auto& scene = engine.getScene();

	bool open = true;

	// Setup window layout
	ImGui::SetNextWindowSize(ImVec2(275, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene", &open, ImGuiWindowFlags_MenuBar);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(375, 425), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties", &open);
	ImGui::End();

	// Draw the system menu
	static bool system_menu = true;
	if (engine.getInput().isKeyPressed(Keyboard::F1)) {
		system_menu = !system_menu;
	}
	if (system_menu) {
		DrawSystemMenu(engine);
	}

	// Draw the FPS display window
	DrawFPSDisplay(engine);

	// Draw scene window contents
	if (ImGui::Begin("Scene")) {

		ModelType add_model_popup = ModelType::None;

		// Draw menu
		DrawSceneMenu(device, resource_mgr, scene, add_model_popup);

		// Process model popup
		ProcNewModelPopups(device, scene, resource_mgr, g_selected_entity, add_model_popup);

		// Draw tree
		DrawTree(scene);
	}
	ImGui::End();
}
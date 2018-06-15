#include "stdafx.h"
#include "user_interface.h"
#include "engine/engine.h"
#include "scene/scene.h"


void* UserInterface::selected = nullptr;
handle64 UserInterface::selected_entity{ handle64::invalid_handle };


void UserInterface::draw(const Engine& engine) {

	auto& scene        = engine.getScene();
	auto& ecs_engine   = engine.getECS();
	auto& device       = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();

	bool open = true;

	// Setup window layout
	ImGui::SetNextWindowSize(ImVec2(275, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene", &open, ImGuiWindowFlags_MenuBar);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties", &open);
	ImGui::End();

	// Draw window contents
	if (ImGui::Begin("Scene")) {

		ModelType add_model_popup = ModelType::None;

		// Draw menu
		drawMenu(device, ecs_engine, resource_mgr, scene, add_model_popup);

		// Process model popup
		procNewModelPopups(device, ecs_engine, resource_mgr, selected_entity, add_model_popup);

		// Draw tree
		drawTree(ecs_engine, scene);
	}
	
	// End window
	ImGui::End();
}




//----------------------------------------------------------------------------------
//
//   Menu
//
//----------------------------------------------------------------------------------

void UserInterface::drawMenu(ID3D11Device& device, ECS& ecs_engine, ResourceMgr& resource_mgr, Scene& scene, ModelType& add_model_popup) const {

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("Entity")) {

			if (ImGui::MenuItem("New")) {
				scene.addEntity<WorldObject<>>(ecs_engine);
			}

			if (ImGui::BeginMenu("Selected")) {

				if (ImGui::BeginMenu("Add Component")) {

					if (ImGui::BeginMenu("Model")) {

						if (ImGui::MenuItem("From file")) {
							wchar_t szFile[512] = {};

							if (OpenFilePicker(szFile, sizeof(szFile))) {
								auto bp = resource_mgr.getOrCreate<ModelBlueprint>(szFile);
								ecs_engine.addComponent<Model>(selected_entity, device, bp);
							}
							else {
								FILE_LOG(logWARNING) << "Failed to open file dialog";
							}
						}

						if (ImGui::BeginMenu("Geometric Shape")) {
							if (ImGui::MenuItem("Cube"))
								add_model_popup = ModelType::Cube;
							if (ImGui::MenuItem("Box"))
								add_model_popup = ModelType::Box;
							if (ImGui::MenuItem("Sphere"))
								add_model_popup = ModelType::Sphere;
							if (ImGui::MenuItem("GeoSphere"))
								add_model_popup = ModelType::GeoSphere;
							if (ImGui::MenuItem("Cylinder"))
								add_model_popup = ModelType::Cylinder;
							if (ImGui::MenuItem("Cone"))
								add_model_popup = ModelType::Cone;
							if (ImGui::MenuItem("Torus"))
								add_model_popup = ModelType::Torus;
							if (ImGui::MenuItem("Tetrahedron"))
								add_model_popup = ModelType::Tetrahedron;
							if (ImGui::MenuItem("Octahedron"))
								add_model_popup = ModelType::Octahedron;
							if (ImGui::MenuItem("Dodecahedron"))
								add_model_popup = ModelType::Dodecahedron;
							if (ImGui::MenuItem("Icosahedron"))
								add_model_popup = ModelType::Icosahedron;

							ImGui::EndMenu();
						}

						ImGui::EndMenu();
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

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Delete")) {
					if (selected_entity != handle64::invalid_handle)
						scene.removeEntity(ecs_engine, selected_entity);
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
//   Object Tree
//
//----------------------------------------------------------------------------------

void UserInterface::drawTree(ECS& ecs_engine, Scene& scene) {

	if (ImGui::BeginChild("object list", ImVec2(250, 0))) {

		const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		                                      | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                                      | ((selected == &scene) ? ImGuiTreeNodeFlags_Selected : 0);

		const bool node_open = ImGui::TreeNodeEx(&scene, node_flags, scene.getName().c_str());

		if (ImGui::IsItemClicked())
			selected = &scene;

		if (node_open) {
			drawTreeNodes(ecs_engine, scene);
		}

		if (selected == &scene) {
			drawDetailsPanel(scene);
		}
	}

	ImGui::EndChild();
}


void UserInterface::drawTreeNodes(ECS& ecs_engine, Scene& scene) {

	auto& entities = scene.getEntities();

	for (const auto entity : entities) {

		string name = "Entity (index: " + to_string(entity.index) + ", counter: " + to_string(entity.counter) + ")";

		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		                                 | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                                 | ((selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0);

		const bool node_open = ImGui::TreeNodeEx(name.c_str(), flags, name.c_str());

		if (ImGui::IsItemClicked())
			selected_entity = entity;

		if (!node_open) continue;

		// Transform
		if (auto* transform = ecs_engine.getComponent<Transform>(entity)) {
			drawNode("Transform", *transform);
		}

		// Perspective Camera
		if (auto* cam = ecs_engine.getComponent<PerspectiveCamera>(entity)) {
			drawNode("Perspective Camera", *cam);
		}

		// Orthographic Camera
		if (auto* cam = ecs_engine.getComponent<OrthographicCamera>(entity)) {
			drawNode("Orthographic Camera", *cam);
		}

		// Camera Movement
		if (auto* cam_movement = ecs_engine.getComponent<CameraMovement>(entity)) {
			drawNode("Camera Movement", *cam_movement);
		}

		// Model
		if (auto* model = ecs_engine.getComponent<Model>(entity)) {
			const bool node_selected = (selected == model);

			const ImGuiTreeNodeFlags model_flags = ImGuiTreeNodeFlags_OpenOnArrow
			                                       | ImGuiTreeNodeFlags_OpenOnDoubleClick
			                                       | (node_selected ? ImGuiTreeNodeFlags_Selected : 0);

			const bool open = ImGui::TreeNodeEx("Model", model_flags);

			if (ImGui::IsItemClicked()) {
				selected = model;
			}

			if (open) {
				model->forEachChild([&](ModelChild& child) {
					bool child_selected = (selected == &child);

					ImGui::Selectable(child.getName().c_str(), &child_selected);

					if (ImGui::IsItemClicked()) {
						selected = &child;
					}

					if (child_selected) {
						drawDetailsPanel(child);
					}
				});

				ImGui::TreePop();
			}

			if (node_selected) {
				drawDetailsPanel(*model);
			}
		}

		// Directional Light
		if (auto* light = ecs_engine.getComponent<DirectionalLight>(entity)) {
			drawNode("Directional Light", *light);
		}

		// Point Light
		if (auto* light = ecs_engine.getComponent<PointLight>(entity)) {
			drawNode("Point Light", *light);
		}

		// Spot Light
		if (auto* light = ecs_engine.getComponent<SpotLight>(entity)) {
			drawNode("Spot Light", *light);
		}

		ImGui::TreePop();
	}

	ImGui::TreePop();
}




//----------------------------------------------------------------------------------
//
//   Object Details
//
//----------------------------------------------------------------------------------


void UserInterface::drawDetails(Scene& scene) const {

	ImGui::Text(scene.getName().c_str());
	ImGui::Separator();

	string entity_count = "Entities: " + to_string(scene.getEntities().size());
	ImGui::Text(entity_count.c_str());
}


void UserInterface::drawDetails(Transform& transform) const {

	ImGui::Text("Transform");
	ImGui::Separator();

	float3 position;
	float3 rotation;
	float3 scale;
	XMStoreFloat3(&position, transform.getPosition());
	XMStoreFloat3(&rotation, transform.getRotation());
	XMStoreFloat3(&scale, transform.getScale());

	if (ImGui::DragFloat3("Position", position.data(), 0.05f, -FLT_MAX, FLT_MAX))
		transform.setPosition(position);

	if (ImGui::DragFloat3("Rotation", rotation.data(), 0.05f, -FLT_MAX, FLT_MAX))
		transform.setRotation(rotation);

	if (ImGui::DragFloat3("Scale", scale.data(), 0.05f, -FLT_MAX, FLT_MAX))
		transform.setScale(scale);
}


void UserInterface::drawDetails(PerspectiveCamera& camera) const {

	ImGui::Text("Fog");
	ImGui::Separator();

	auto& fog = camera.getFog();
	float4 color = fog.color;
	float  start = fog.start;
	float  range = fog.range;

	bool update = false;

	if (ImGui::ColorEdit4("Color", color.data()))
		update = true;
	
	if (ImGui::InputFloat("Start", &start))
		update = true;

	if (ImGui::InputFloat("Range", &range))
		update = true;

	if (update)
		camera.setFog(Fog(color, start, range));
}

void UserInterface::drawDetails(CameraMovement& movement) const {

	ImGui::Text("Movement");
	ImGui::Separator();

	float max_velocity = movement.getMaxVelocity();
	float acceleration = movement.getAcceleration();
	float deceleration = movement.getDeceleration();
	float turn_sensitivity = movement.getTurnSensitivity();
	float roll_sensitivity = movement.getRollSensitivity();

	if (ImGui::InputFloat("Max Velocity", &max_velocity))
		movement.setMaxVelocity(max_velocity);

	if (ImGui::InputFloat("Acceleration", &acceleration))
		movement.setAcceleration(acceleration);

	if (ImGui::InputFloat("Deceleration", &deceleration))
		movement.setDeceleration(deceleration);

	if (ImGui::InputFloat("Turn Sensitivity", &turn_sensitivity))
		movement.setTurnSensitivity(turn_sensitivity);

	if (ImGui::InputFloat("Sensitivity", &roll_sensitivity))
		movement.setRollSensitivity(roll_sensitivity);
}


void UserInterface::drawDetails(OrthographicCamera& camera) const {
	
}



void UserInterface::drawDetails(Model& model) const {
	
}


void UserInterface::drawDetails(ModelChild& child) const {

	ImGui::Text(child.getName().c_str());
	ImGui::Separator();

	// Change material properties, textures
	string name = "Material - " + child.getMaterial().name;
	ImGui::Text(name.c_str());

	ImGui::ColorEdit3("Diffuse Color",      (float*)child.getMaterial().diffuse.data());
	ImGui::ColorEdit3("Ambient Color",      (float*)child.getMaterial().ambient.data());
	ImGui::ColorEdit3("Specular Color",     (float*)child.getMaterial().specular.data());
	ImGui::DragFloat( "Specular Exponent",  (float*)&child.getMaterial().specular.w, 0.01f, 0.0f, FLT_MAX);
	ImGui::ColorEdit3("Reflective Color",   (float*)child.getMaterial().reflect.data());
	ImGui::Checkbox(  "Reflection",         (bool*)&child.getMaterial().reflection_enabled);
}


void UserInterface::drawDetails(DirectionalLight& light) const {

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

	auto shadows = light.castsShadows();
	if (ImGui::Checkbox("Shadows", &shadows))
		light.setShadows(shadows);
}


void UserInterface::drawDetails(PointLight& light) const {

	ImGui::Text("Point Light");
	ImGui::Separator();

	float range = light.getRange();
	if (ImGui::DragFloat( "Range", &range, 0.1f,  0.0f, FLT_MAX))
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


void UserInterface::drawDetails(SpotLight& light) const {

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




//----------------------------------------------------------------------------------
//
//   Popup Windows
//
//----------------------------------------------------------------------------------

void UserInterface::procNewModelPopups(ID3D11Device& device, ECS& ecs_engine, ResourceMgr& resource_mgr, handle64 entity, ModelType type) const {

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
		static float size = 1.0f;
		static bool  rhcoords = false;
		static bool  invertn = false;

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
		static float3 size = { 1.0f, 1.0f, 1.0f };
		static bool   rhcoords = false;
		static bool   invertn = false;

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
		static float  diameter = 1.0f;
		static size_t tessellation = 16;
		static bool   rhcoords = false;
		static bool   invertn = false;

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
		static float  diameter = 1.0f;
		static size_t tessellation = 3;
		static bool   rhcoords = false;

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
		static float  height = 1.0f;
		static float  diameter = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords = false;

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
		static float  height = 1.0f;
		static float  diameter = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords = false;

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
		static float  thickness = 0.333f;
		static float  diameter = 1.0f;
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
		static float size = 1.0f;
		static bool  rhcoords = false;

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
		static float size = 1.0f;
		static bool  rhcoords = false;

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
		static float size = 1.0f;
		static bool  rhcoords = false;

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
		static float size = 1.0f;
		static bool  rhcoords = false;

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

#include "stdafx.h"
#include "user_interface.h"
#include "system\system.h"
#include "scene\scene.h"


static void* selected = 0;


void UserInterface::Draw(System& system) {

	Scene& scene        = system.GetScene();
	auto*  device       = system.GetRenderingMgr().GetDevice();
	auto&  resource_mgr = system.GetRenderingMgr().GetResourceMgr();

	bool open = true;
	
	ImGui::SetNextWindowSize(ImVec2(550, 600), ImGuiCond_FirstUseEver);

	// Begin window
	if (ImGui::Begin("Scene", &open, ImGuiWindowFlags_MenuBar)) {

		// Popup enum determines if any popups need to be drawn
		Popups popup = Popups::Null;

		// Draw Menu
		DrawMenu(device, resource_mgr, scene, popup);

		// Left pane (object list)
		DrawObjectLists(scene);

		// Keep next child on the same line
		ImGui::SameLine();

		// Right pane (object properties)
		DrawObjectDetails(scene);
		
		// Draw the opened popup windows
		DrawPopups(device, scene, popup);
	}
	
	// End window
	ImGui::End();
}




//----------------------------------------------------------------------------------
//
//   Menu
//
//----------------------------------------------------------------------------------

void UserInterface::DrawMenu(ID3D11Device* device, ResourceMgr& resource_mgr, Scene& scene, Popups& popup) {

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("Lights")) {

			if (ImGui::MenuItem("Add Dir. Light")) {
				scene.GetDirectionalLights().push_back(DirectionalLight());
			}

			if (ImGui::MenuItem("Add Point Light")) {
				scene.GetPointLights().push_back(PointLight());
			}

			if (ImGui::MenuItem("Add Spot Light")) {
				scene.GetSpotLights().push_back(SpotLight());
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Models")) {

			if (ImGui::BeginMenu("Add Model")) {

				if (ImGui::MenuItem("From file")) {
					wchar_t szFile[512] = {};

					if (OpenFilePicker(szFile, sizeof(szFile))) {
						auto bp = resource_mgr.Create<ModelBlueprint>(szFile);
						scene.GetModels().push_back(Model(device, bp));
					}
					else {
						FILE_LOG(logWARNING) << "Failed to open file dialog";
					}
				}

				if (ImGui::MenuItem("Copy existing")) {
					popup = Popups::NewModelCopy;
				}

				if (ImGui::BeginMenu("Geometric Shape")) {
					if (ImGui::MenuItem("Cube"))
						popup = Popups::NewCube;
					if (ImGui::MenuItem("Box"))
						popup = Popups::NewBox;
					if (ImGui::MenuItem("Sphere"))
						popup = Popups::NewSphere;
					if (ImGui::MenuItem("GeoSphere"))
						popup = Popups::NewGeoSphere;
					if (ImGui::MenuItem("Cylinder"))
						popup = Popups::NewCylinder;
					if (ImGui::MenuItem("Cone"))
						popup = Popups::NewCone;
					if (ImGui::MenuItem("Torus"))
						popup = Popups::NewTorus;
					if (ImGui::MenuItem("Tetrahedron"))
						popup = Popups::NewTetrahedron;
					if (ImGui::MenuItem("Octahedron"))
						popup = Popups::NewOctahedron;
					if (ImGui::MenuItem("Dodecahedron"))
						popup = Popups::NewDodecahedron;
					if (ImGui::MenuItem("Icosahedron"))
						popup = Popups::NewIcosahedron;

					ImGui::EndMenu();
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

void UserInterface::DrawObjectLists(Scene& scene) {

	if (ImGui::BeginChild("object list", ImVec2(250, 0))) {

		// Populate list with models
		DrawModelList(scene);

		// Populate list with lights
		DrawLightList(scene);

		// Camera
		ImGui::Selectable("Camera", selected == &scene.GetCamera());
		if (ImGui::IsItemClicked())
			selected = &scene.GetCamera();

		// Fog
		ImGui::Selectable("Fog", selected == &scene.GetFog());
		if (ImGui::IsItemClicked()) {
			selected = &scene.GetFog();
		}
	}

	ImGui::EndChild();
}


void UserInterface::DrawModelList(Scene& scene) {

	// Draw the tree node and check if it's open
	if (ImGui::TreeNode("Models")) {

		// Draw a node for each model
		scene.ForEach<Model>([&](Model& model) {
			ImGui::PushID(&model);

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selected == &model) ? ImGuiTreeNodeFlags_Selected : 0);

			bool node_open = ImGui::TreeNodeEx(&model, node_flags, model.GetName().c_str());

			// Check if the node is selected
			if (ImGui::IsItemClicked())
				selected = &model;

			// Draw a node for each child model
			if (node_open) {
				model.ForEachChild([&](ModelChild& child) {

					ImGui::PushID(&child);
					ImGui::Bullet();

					ImGui::Selectable(child.GetName().c_str(), selected == &child);

					if (ImGui::IsItemClicked())
						selected = &child;

					ImGui::PopID();
				});

				ImGui::TreePop();
			}

			ImGui::PopID();
		});

		ImGui::TreePop();
	}
}


void UserInterface::DrawLightList(Scene& scene) {
	//----------------------------------------------------------------------------------
	// Directional Lights
	//----------------------------------------------------------------------------------
	bool node_open = ImGui::TreeNode("Directional Lights");

	if (node_open) {
		scene.ForEach<DirectionalLight>([&](auto& light) {
			ImGui::PushID(&light);

			char name[28];
			sprintf_s(name, "Dir. Light %p", (void*)&light);
			ImGui::Selectable(name, selected == &light);

			if (ImGui::IsItemClicked()) {
				selected = &light;
			}

			ImGui::PopID();
		});

		ImGui::TreePop();
	}


	//----------------------------------------------------------------------------------
	// Point Lights
	//----------------------------------------------------------------------------------
	node_open = ImGui::TreeNode("Point Lights");

	if (node_open) {
		scene.ForEach<PointLight>([&](auto& light) {
			ImGui::PushID(&light);

			char name[29];
			sprintf_s(name, "Point Light %p", (void*)&light);
			ImGui::Selectable(name, selected == &light);

			if (ImGui::IsItemClicked()) {
				selected = &light;
			}

			ImGui::PopID();
		});

		ImGui::TreePop();
	}


	//----------------------------------------------------------------------------------
	// Spot Lights
	//----------------------------------------------------------------------------------
	node_open = ImGui::TreeNode("Spot Lights");

	if (node_open) {
		scene.ForEach<SpotLight>([&](auto& light) {
			ImGui::PushID(&light);

			char name[28];
			sprintf_s(name, "Spot Light %p", (void*)&light);
			ImGui::Selectable(name, selected == &light);

			if (ImGui::IsItemClicked()) {
				selected = &light;
			}

			ImGui::PopID();
		});

		ImGui::TreePop();
	}
}




//----------------------------------------------------------------------------------
//
//   Object Details
//
//----------------------------------------------------------------------------------

void UserInterface::DrawObjectDetails(Scene& scene) {

	ImGui::BeginChild("object properties", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);

	// Draw model details
	scene.ForEach<Model>([&](Model& model) {
		if (selected == &model) {
			DrawModelDetails(model);

			ImGui::Separator();
			ImGui::Button("Delete Model");

			if (ImGui::IsItemClicked()) {
				auto& models = scene.GetModels();
				models.erase(models.begin() + (&model - models.data()));
			}
		}
		else {
			model.ForEachChild([&](auto& child) {
				if (selected == &child)
					DrawChildModelDetails(child);
			});
		}
	});


	// Directional light details
	scene.ForEach<DirectionalLight>([&](auto& light) {
		if (selected == &light) {
			DrawLightDetails(light);

			ImGui::Separator();
			ImGui::Button("Delete Light");

			if (ImGui::IsItemClicked()) {
				auto& lights = scene.GetDirectionalLights();
				lights.erase(lights.begin() + (&light - lights.data()));
			}
		}
	});

	// Point light details
	scene.ForEach<PointLight>([&](auto& light) {
		if (selected == &light) {
			DrawLightDetails(light);

			ImGui::Separator();
			ImGui::Button("Delete Light");

			if (ImGui::IsItemClicked()) {
				auto& lights = scene.GetPointLights();
				lights.erase(lights.begin() + (&light - lights.data()));
			}
		}
	});

	// Spot light details
	scene.ForEach<SpotLight>([&](auto& light) {
		if (selected == &light) {
			DrawLightDetails(light);

			ImGui::Separator();
			ImGui::Button("Delete Light");

			if (ImGui::IsItemClicked()) {
				auto& lights = scene.GetSpotLights();
				lights.erase(lights.begin() + (&light - lights.data()));
			}
		}
	});


	// Draw camera details
	if (selected == &scene.GetCamera())
		DrawCameraDetails(scene.GetCamera());


	// Draw fog details
	if (selected == &scene.GetFog()) {
		ImGui::DragFloat3("Color",  scene.GetFog().color.Data(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat( "Start", &scene.GetFog().start,        0.05f, 0.0f, FLT_MAX);
		ImGui::DragFloat( "Range", &scene.GetFog().range,        0.05f, 0.0f, FLT_MAX);
	}


	ImGui::EndChild();
}


void UserInterface::DrawCameraDetails(PlayerCamera& camera) {

	float max_velocity = camera.GetMaxVelocity();
	float acceleration = camera.GetAcceleration();
	float deceleration = camera.GetDeleceration();
	float sensitivity  = camera.GetSensitivity();

	if (ImGui::InputFloat("Max Velocity", &max_velocity))
		camera.SetMaxVelocity(max_velocity);

	if (ImGui::InputFloat("Acceleration", &acceleration))
		camera.SetAcceleration(acceleration);

	if (ImGui::InputFloat("Deceleration", &deceleration))
		camera.SetDeceleration(deceleration);

	if (ImGui::InputFloat("Sensitivity", &sensitivity))
		camera.SetSensitivity(sensitivity);


	bool free_look = camera.IsFreeLookEnabled();

	if (ImGui::Checkbox("Free Look", &free_look))
		camera.SetFreeLook(free_look);
}


void UserInterface::DrawModelDetails(Model& model) {

	ImGui::Text(model.GetName().c_str());
	ImGui::Separator();


	auto& t = model.GetTransform();

	float3 position;
	float3 rotation;
	float3 scale;
	XMStoreFloat3(&position, t.GetPosition());
	XMStoreFloat3(&rotation, t.GetRotation());
	XMStoreFloat3(&scale, t.GetScale());

	if (ImGui::DragFloat3("Position", position.Data(), 0.01f, -FLT_MAX, FLT_MAX))
		model.SetPosition(position);

	if (ImGui::DragFloat3("Rotation", rotation.Data(), 0.01f, -FLT_MAX, FLT_MAX))
		model.SetRotation(rotation);

	if (ImGui::DragFloat3("Scale", scale.Data(), 0.01f, -FLT_MAX, FLT_MAX))
		model.SetScale(scale);
}


void UserInterface::DrawChildModelDetails(ModelChild& child) {

	ImGui::Text(child.GetName().c_str());
	ImGui::Separator();

	// Change material properties, textures
	string name = "Material - " + child.GetMaterial().name;
	ImGui::Text(name.c_str());
	ImGui::ColorEdit3("Diffuse Color",     (float*)&child.GetMaterial().Kd);
	ImGui::ColorEdit3("Ambient Color",     (float*)&child.GetMaterial().Ka);
	ImGui::ColorEdit3("Specular Color",    (float*)&child.GetMaterial().Ks);
	ImGui::DragFloat( "Specular Exponent", (float*)&child.GetMaterial().Ks.w, 0.01f, 0.0f, FLT_MAX);
}


void UserInterface::DrawLightDetails(DirectionalLight& light) {

	char name[28];
	sprintf_s(name, "Dir. Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Direction",      light.direction.Data(), 0.01f, -1.0f, 1.0f);
	ImGui::NewLine();
	ImGui::ColorEdit3("Diffuse Color",  light.diffuse_color.Data());
	ImGui::ColorEdit3("Ambient Color",  light.ambient_color.Data());
	ImGui::ColorEdit3("Specular Color", light.specular.Data());
}


void UserInterface::DrawLightDetails(PointLight& light) {

	char name[29];
	sprintf_s(name, "Point Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Position",       light.position.Data(),    0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat( "Range",         &light.range,              0.1f,  0.0f,    FLT_MAX);
	ImGui::DragFloat3("Attenuation",    light.attenuation.Data(), 0.01f, 0.0f,    1.0f);
	ImGui::NewLine();
	ImGui::ColorEdit3("Diffuse Color",  light.diffuse_color.Data());
	ImGui::ColorEdit3("Ambient Color",  light.ambient_color.Data());
	ImGui::ColorEdit3("Specular Color", light.specular.Data());
}


void UserInterface::DrawLightDetails(SpotLight& light) {

	char name[28];
	sprintf_s(name, "Spot Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Position",       light.position.Data(),    0.1f,  -FLT_MAX, FLT_MAX);
	ImGui::DragFloat3("Direction",      light.direction.Data(),   0.01f, -1.0f,    1.0f);
	ImGui::DragFloat( "Range",         &light.range,              0.1f,   0.0f,    FLT_MAX);
	ImGui::DragFloat( "Spot",          &light.spot,               0.05f,  0.0f,    FLT_MAX);
	ImGui::DragFloat3("Attenuation",    light.attenuation.Data(), 0.01f,  0.0f,    1.0f);
	ImGui::NewLine();
	ImGui::ColorEdit3("Diffuse Color",  light.diffuse_color.Data());
	ImGui::ColorEdit3("Ambient Color",  light.ambient_color.Data());
	ImGui::ColorEdit3("Specular Color", light.specular.Data());
}




//----------------------------------------------------------------------------------
//
//   Popup Windows
//
//----------------------------------------------------------------------------------

void UserInterface::DrawPopups(ID3D11Device* device, Scene& scene, Popups popup) {

	switch (popup) {
		case Popups::NewModelCopy:
			ImGui::OpenPopup("New Model Copy");
			break;
		case Popups::NewCube:
			ImGui::OpenPopup("New Cube");
			break;
		case Popups::NewBox:
			ImGui::OpenPopup("New Box");
			break;
		case Popups::NewSphere:
			ImGui::OpenPopup("New Sphere");
			break;
		case Popups::NewGeoSphere:
			ImGui::OpenPopup("New GeoSphere");
			break;
		case Popups::NewCylinder:
			ImGui::OpenPopup("New Cylinder");
			break;
		case Popups::NewTorus:
			ImGui::OpenPopup("New Torus");
			break;
		case Popups::NewCone:
			ImGui::OpenPopup("New Cone");
			break;
		case Popups::NewTetrahedron:
			ImGui::OpenPopup("New Tetrahedron");
			break;
		case Popups::NewOctahedron:
			ImGui::OpenPopup("New Octahedron");
			break;
		case Popups::NewDodecahedron:
			ImGui::OpenPopup("New Dodecahedron");
			break;
		case Popups::NewIcosahedron:
			ImGui::OpenPopup("New Icosahedron");
			break;
	}


	if (ImGui::BeginPopupModal("New Model Copy", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		static int selected_combo = NULL;

		static auto getter = [](void* vec, int idx, const char** out_text) {
			auto& vector = *static_cast<std::vector<Model>*>(vec);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
			*out_text = vector.at(idx).GetName().c_str();
			return true;
		};

		ImGui::Combo("Models", &selected_combo, getter, (void*)&scene.GetModels(), scene.GetModels().size());

		if (ImGui::Button("Ok")) {
			scene.GetModels().push_back(Model(scene.GetModels().at(selected_combo)));
		}

		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cube", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float size = 1.0f;
		static bool  rhcoords = false;
		static bool  invertn = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);
		ImGui::Checkbox("Invert Normals", &invertn);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateCube<VertexPositionNormalTexture>(device, size, rhcoords, invertn);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Box", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float3 size = { 1.0f, 1.0f, 1.0f };
		static bool   rhcoords = false;
		static bool   invertn = false;

		ImGui::InputFloat3("Size", size.Data());
		ImGui::Checkbox("Right-hand Coords", &rhcoords);
		ImGui::Checkbox("Invert Normals", &invertn);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateBox<VertexPositionNormalTexture>(device, size, rhcoords, invertn);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Sphere", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float  diameter = 1.0f;
		static size_t tessellation = 16;
		static bool   rhcoords = false;
		static bool   invertn = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);
		ImGui::Checkbox("Invert Normals", &invertn);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateSphere<VertexPositionNormalTexture>(device, diameter, tessellation, rhcoords, invertn);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New GeoSphere", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float  diameter = 1.0f;
		static size_t tessellation = 3;
		static bool   rhcoords = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 8) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is extremely high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateGeoSphere<VertexPositionNormalTexture>(device, diameter, tessellation, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cylinder", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float  height = 1.0f;
		static float  diameter = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 128) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCylinder<VertexPositionNormalTexture>(device, diameter, height, tessellation, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cone", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float  height = 1.0f;
		static float  diameter = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 128) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCone<VertexPositionNormalTexture>(device, diameter, height, tessellation, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Torus", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float  thickness = 0.333f;
		static float  diameter = 1.0f;
		static size_t tessellation = 32;
		static bool   rhcoords = false;

		ImGui::InputFloat("Thickness", &thickness);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 128) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateTorus<VertexPositionNormalTexture>(device, diameter, thickness, tessellation, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Tetrahedron", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float size = 1.0f;
		static bool  rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateTetrahedron<VertexPositionNormalTexture>(device, size, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Octahedron", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float size = 1.0f;
		static bool  rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateOctahedron<VertexPositionNormalTexture>(device, size, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Dodecahedron", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float size = 1.0f;
		static bool  rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateDodecahedron<VertexPositionNormalTexture>(device, size, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Icosahedron", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static float size = 1.0f;
		static bool  rhcoords = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Right-hand Coords", &rhcoords);

		if (ImGui::Button("Create")) {
			auto bp = BlueprintFactory::CreateIcosahedron<VertexPositionNormalTexture>(device, size, rhcoords);
			scene.GetModels().push_back(Model(device, bp));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
#include "stdafx.h"
#include "user_interface.h"
#include "scene\scene.h"


static void* selected = 0;


void UserInterface::Draw(Scene& scene) {
	
	ImGui::SetNextWindowSize(ImVec2(550, 600), ImGuiCond_FirstUseEver);

	bool open = true;

	// Begin window
	if (ImGui::Begin("Scene", &open, ImGuiWindowFlags_MenuBar)) {

		// Draw Menu
		DrawMenu(scene);

		// Left pane (object list)
		DrawObjectList(scene);

		// Keep next child on the same line
		ImGui::SameLine();

		// Right pane (object properties)
		DrawObjectDetails(scene);
	}
	
	// End window
	ImGui::End();
}


void UserInterface::DrawMenu(Scene& scene) {

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

		ImGui::EndMenuBar();
	}
}


void UserInterface::DrawObjectList(Scene& scene) {

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

	bool node_open = ImGui::TreeNode("Models");

	if (node_open) {
		scene.ForEach<Model>([&](Model& model) {
			ImGui::PushID(&model);

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selected == &model) ? ImGuiTreeNodeFlags_Selected : 0);

			bool node_open = ImGui::TreeNodeEx(&model, node_flags, model.GetName().c_str());

			if (ImGui::IsItemClicked())
				selected = &model;

			if (node_open) {
				model.ForEachChild([&](ChildModel& child) {

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


void UserInterface::DrawObjectDetails(Scene& scene) {

	ImGui::BeginChild("object properties", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

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
		ImGui::DragFloat3("Color", scene.GetFog().color.RawData(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat( "Start", &scene.GetFog().start,          0.05f, 0.0f, FLT_MAX);
		ImGui::DragFloat( "Range", &scene.GetFog().range,          0.05f, 0.0f, FLT_MAX);
	}


	ImGui::EndChild();
}


void UserInterface::DrawCameraDetails(Camera& camera) {
	// Set max veloctiy, accel rate, decel rate
	// Change camera mode
}


void UserInterface::DrawModelDetails(Model& model) {

	ImGui::Text(model.GetName().c_str());
	ImGui::Separator();

	// Set position, rotation, scale

	//auto& m = model.GetTransform();
	//float pos[3] = { XMVectorGetX(m.r[3]), XMVectorGetY(m.r[3]), XMVectorGetZ(m.r[3]) };
	//ImGui::InputFloat3("Position", (float*)&pos);
}


void UserInterface::DrawChildModelDetails(ChildModel& child) {

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

	ImGui::DragFloat3("Direction",      light.direction.RawData(), 0.01f, -1.0f, 1.0f);
	ImGui::ColorEdit3("Diffuse Color",  light.diffuse_color.RawData());
	ImGui::ColorEdit3("Ambient Color",  light.ambient_color.RawData());
	ImGui::ColorEdit3("Specular Color", light.specular.RawData());
}


void UserInterface::DrawLightDetails(PointLight& light) {

	char name[29];
	sprintf_s(name, "Point Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Position",       light.position.RawData(),    0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat( "Range",         &light.range,                 0.1f,  0.0f, FLT_MAX);
	ImGui::DragFloat3("Attenuation",    light.attenuation.RawData(), 0.01f, 0.0f, 1.0f);
	ImGui::ColorEdit3("Diffuse Color",  light.diffuse_color.RawData());
	ImGui::ColorEdit3("Ambient Color",  light.ambient_color.RawData());
	ImGui::ColorEdit3("Specular Color", light.specular.RawData());
}


void UserInterface::DrawLightDetails(SpotLight& light) {

	char name[28];
	sprintf_s(name, "Spot Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Position",       light.position.RawData(),    0.1f,  -FLT_MAX, FLT_MAX);
	ImGui::DragFloat3("Direction",      light.direction.RawData(),   0.01f, -1.0f, 1.0f);
	ImGui::DragFloat( "Range",         &light.range,                 0.1f,   0.0f, FLT_MAX);
	ImGui::DragFloat( "Spot",          &light.spot,                  0.05f,  0.0f, FLT_MAX);
	ImGui::DragFloat3("Attenuation",    light.attenuation.RawData(), 0.01f,  0.0f, 1.0f);
	ImGui::ColorEdit3("Diffuse Color",  light.diffuse_color.RawData());
	ImGui::ColorEdit3("Ambient Color",  light.ambient_color.RawData());
	ImGui::ColorEdit3("Specular Color", light.specular.RawData());
}
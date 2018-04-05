#include "stdafx.h"
#include "ui_renderer.h"


static void* selected = 0;


void UIRenderer::Render(Scene& scene) {
	
	ImGui::SetNextWindowSize(ImVec2(550, 600), ImGuiCond_FirstUseEver);

	// Begin window
	if (ImGui::Begin("Scene")) {

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


void UIRenderer::DrawObjectList(Scene& scene) {

	if (ImGui::BeginChild("object list", ImVec2(250, 0))) {

		// Populate list with models
		DrawModelList(scene);

		// Populate list with lights
		DrawLightList(scene);

		// Camera
		ImGui::Selectable("Camera", selected == &scene.GetCamera());
		if (ImGui::IsItemClicked())
			selected = &scene.GetCamera();
	}

	ImGui::EndChild();
}


void UIRenderer::DrawObjectDetails(Scene& scene) {

	ImGui::BeginChild("object properties", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

	// Draw model details
	scene.ForEach<Model>([&](Model& model) {
		if (selected == &model) {
			DrawModelDetails(model);
		}
		else {
			model.ForEachChild([&](auto& child) {
				if (selected == &child)
					DrawChildModelDetails(child);
			});
		}
	});


	// Draw light details
	scene.ForEach<DirectionalLight>([&](auto& light) {
		if (selected == &light)
			DrawLightDetails(light);
	});

	scene.ForEach<PointLight>([&](auto& light) {
		if (selected == &light)
			DrawLightDetails(light);
	});

	scene.ForEach<SpotLight>([&](auto& light) {
		if (selected == &light)
			DrawLightDetails(light);
	});


	// Draw camera details
	if (selected == &scene.GetCamera())
		DrawCameraDetails(scene.GetCamera());


	ImGui::EndChild();
}


void UIRenderer::DrawModelList(Scene& scene) {

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


void UIRenderer::DrawLightList(Scene& scene) {
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
			ImGui::Selectable(name);

			if (ImGui::IsItemClicked(), selected == &light) {
				selected = &light;
			}

			ImGui::PopID();
		});

		ImGui::TreePop();
	}
}


void UIRenderer::DrawCameraDetails(Camera& camera) {
	// Set max veloctiy, accel rate, decel rate
	// Change camera mode
}


void UIRenderer::DrawModelDetails(Model& model) {

	ImGui::Text(model.GetName().c_str());
	ImGui::Separator();

	// Set position, rotation, scale

	//auto& m = model.GetTransform();
	//float pos[3] = { XMVectorGetX(m.r[3]), XMVectorGetY(m.r[3]), XMVectorGetZ(m.r[3]) };
	//ImGui::InputFloat3("Position", (float*)&pos);
}


void UIRenderer::DrawChildModelDetails(ChildModel& child) {

	ImGui::Text(child.GetName().c_str());
	ImGui::Separator();

	// Change material properties, textures
	ImGui::Text("Material");
	ImGui::DragFloat3("Diffuse Color",     (float*)&child.GetMaterial().Kd,   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Ambient Color",     (float*)&child.GetMaterial().Ka,   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Specular Color",    (float*)&child.GetMaterial().Ks,   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat( "Specular Exponent", (float*)&child.GetMaterial().Ks.w, 0.01f, 0.0f, FLT_MAX);
}


void UIRenderer::DrawLightDetails(DirectionalLight& light) {

	char name[28];
	sprintf_s(name, "Dir. Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Direction",      light.direction.RawData(),     0.01f, -1.0f, 1.0f);
	ImGui::DragFloat3("Diffuse Color",  light.diffuse_color.RawData(), 0.01f,  0.0f, 1.0f);
	ImGui::DragFloat3("Ambient Color",  light.ambient_color.RawData(), 0.01f,  0.0f, 1.0f);
	ImGui::DragFloat4("Specular Color", light.specular.RawData(),      0.01f,  0.0f, 1.0f);
}


void UIRenderer::DrawLightDetails(PointLight& light) {

	char name[29];
	sprintf_s(name, "Point Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Position",       light.position.RawData(),        0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat( "Range",         &light.range,                     0.1f,  0.0f, FLT_MAX);
	ImGui::DragFloat3("Attenuation",    light.attenuation.RawData(),     0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Diffuse Color",  light.diffuse_color.RawData(),   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Ambient Color",  light.ambient_color.RawData(),   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Specular Color", light.specular.RawData(),        0.01f, 0.0f, 1.0f);
}


void UIRenderer::DrawLightDetails(SpotLight& light) {

	char name[28];
	sprintf_s(name, "Spot Light %p", (void*)&light);

	ImGui::Text(name);
	ImGui::Separator();

	ImGui::DragFloat3("Position",       light.position.RawData(),        0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat( "Range",         &light.range,                     0.1f,  0.0f, FLT_MAX);
	ImGui::DragFloat( "Spot",          &light.spot,                      0.05f, 0.0f, FLT_MAX);
	ImGui::DragFloat3("Attenuation",    light.attenuation.RawData(),     0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Diffuse Color",  light.diffuse_color.RawData(),   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Ambient Color",  light.ambient_color.RawData(),   0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Specular Color", light.specular.RawData(),        0.01f, 0.0f, 1.0f);
}
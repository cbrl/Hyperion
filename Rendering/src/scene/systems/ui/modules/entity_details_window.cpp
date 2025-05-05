module;

#include <DirectXMath.h>
#include <gsl/gsl>

#include <imgui.h>

#include "io/io.h"
#include "misc/cpp/imgui_stdlib.h"
#include "memory/handle/handle.h"
#include "string/string.h"

module rendering;

import math.directxmath;
import log;
import win_utils;

import :engine;
import :importer.model_importer;
import :texture_factory;
import :resource_mgr;
import :components.hierarchy;
import :components.text;
import :components.transform;
import :components.light.ambient_light;
import :components.light.directional_light;
import :components.light.point_light;
import :components.light.spot_light;


using namespace DirectX;
using namespace render; // render namespace is heavily utilized here, so import it for brevity.


template<typename ComponentT>
void DrawComponentState(ComponentT& component, gsl::czstring name = nullptr) {
	if (name) {
		ImGui::InputText("", const_cast<char*>(name), std::strlen(name), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
	}
	bool state = component.isActive();
	if (ImGui::Checkbox("Active", &state))
		component.setActive(state);
	ImGui::Spacing();
}


void EntityDetailsWindow::draw(Engine& engine, handle64 handle) {

	auto& device       = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto& scene        = engine.getScene();
	auto& ecs          = scene.getECS();

	// Begin window. Early return if window isn't open or entity isn't valid.
	ImGui::SetNextWindowSize(ImVec2{375, 425}, ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Entity Properties", nullptr, ImGuiWindowFlags_MenuBar) || !ecs.valid(handle)) {
		ImGui::End();
		return;
	}

	// Draw menu
	if (ImGui::BeginMenuBar()) {
		drawAddComponentMenu(engine, ecs, handle);
		ImGui::EndMenuBar();
	}

	// Render "New Model" popup windows
	new_model_menu.procNewModelPopup(device, resource_mgr, scene, handle);


	//----------------------------------------------------------------------------------
	// Name/Details
	//----------------------------------------------------------------------------------
	if (auto* name = ecs.tryGet<Name>(handle)) {
		ImGui::InputText("", &name->name);
		ImGui::Separator();
	}

	ImGui::Text("Index:   %d", handle.index);
	ImGui::Text("Counter: %d", handle.counter);
	ImGui::Spacing();


	//----------------------------------------------------------------------------------
	// Parent
	//----------------------------------------------------------------------------------
	auto* hierarchy = ecs.tryGet<Hierarchy>(handle);

	entity_list.clear();
	scene.getECS().forEach([&](handle64 other_entity) {
		entity_list.push_back(other_entity);
	});

	const char* preview = "None";
	if (hierarchy && ecs.valid(hierarchy->getParent())) {
		if (auto* name = ecs.tryGet<Name>(hierarchy->getParent())) {
			preview = name->name.c_str();
		}
	}

	if (ImGui::BeginCombo("Parent", preview)) {
		if ( ImGui::Selectable("None", !hierarchy || (hierarchy->getParent() == handle64::invalid_handle())) ) {
			if (hierarchy)
				hierarchy->removeParent(ecs);
		}

		for (size_t i = 0; i < entity_list.size(); ++i) {
			const bool selected = (entity_names_idx == i);
			bool set_parent = false;

			if (auto* name = ecs.tryGet<Name>(entity_list[i])) {
				if (ImGui::Selectable(name->name.c_str(), selected)) {
					set_parent = true;
				}
			}
			else {
				const std::string temp_name = "(Index: " + ToStr(entity_list[i].index).value_or("-1"s) +
				                              ", Counter: " + ToStr(entity_list[i].counter).value_or("-1"s) + ")";
				if (ImGui::Selectable(temp_name.c_str(), selected)) {
					set_parent = true;
				}
			}

			if (set_parent) {
				entity_names_idx = static_cast<int>(i);

				if (not hierarchy) {
					hierarchy = &ecs.add<Hierarchy>(handle);
				}
				hierarchy->setParent(ecs, entity_list[i]);
			}
		}

		ImGui::EndCombo();
	}


	//----------------------------------------------------------------------------------
	// Draw Component Nodes
	//----------------------------------------------------------------------------------

	// Render details of user-defined components if applicable
	for (const auto& [idx, component_def] : user_components) {
		if (component_def.getter) {
			auto* component = component_def.getter(ecs, handle);
			if (component) {
				drawUserComponentNode(ecs, component_def.name.c_str(), *component, component_def.details_renderer);
			}
		}
	}

	// Transform
	if (auto* transform = ecs.tryGet<Transform>(handle)) {
		drawComponentNode(ecs, "Transform", *transform);
	}

	// Model
	if (auto* model = ecs.tryGet<Model>(handle)) {
		const std::string name = "Model: " + model->getName();
		drawComponentNode(ecs, name.c_str(), *model, resource_mgr);
	}

	// Perspective Camera
	if (auto* camera = ecs.tryGet<PerspectiveCamera>(handle)) {
		drawComponentNode(ecs, "Perspective Camera", *camera);
	}

	// Orthographic Camera
	if (auto* camera = ecs.tryGet<OrthographicCamera>(handle)) {
		drawComponentNode(ecs, "Orthographic Camera", *camera);
	}

	// Text
	if (auto* text = ecs.tryGet<Text>(handle)) {
		drawComponentNode(ecs, "Text", *text);
	}

	// Ambient Light
	if (auto* light = ecs.tryGet<AmbientLight>(handle)) {
		drawComponentNode(ecs, "Ambient Light", *light);
	}

	// Directional Light
	if (auto* light = ecs.tryGet<DirectionalLight>(handle)) {
		drawComponentNode(ecs, "Directional Light", *light);
	}

	// Point Light
	if (auto* light = ecs.tryGet<PointLight>(handle)) {
		drawComponentNode(ecs, "Point Light", *light);
	}

	// Spot Light
	if (auto* light = ecs.tryGet<SpotLight>(handle)) {
		drawComponentNode(ecs, "Spot Light", *light);
	}

	ImGui::End(); //"Properties"
}


void EntityDetailsWindow::drawAddComponentMenu(Engine& engine, ecs::ECS& ecs, handle64 handle) {

	bool valid_entity = ecs.valid(handle);

	auto& scene         = engine.getScene();
	auto& rendering_mgr = engine.getRenderingMgr();
	auto& device        = rendering_mgr.getDevice();
	auto& resource_mgr  = rendering_mgr.getResourceMgr();

	if (ImGui::BeginMenu("Add Component", valid_entity)) {

		if (ImGui::MenuItem("Name", nullptr, nullptr, !ecs.has<Name>(handle))) {
			ecs.add<Name>(handle);
		}
		if (ImGui::MenuItem("Transform", nullptr, nullptr, !ecs.has<Transform>(handle))) {
			ecs.add<Transform>(handle);
		}
		if (ImGui::MenuItem("Orthographic Camera", nullptr, nullptr, !ecs.has<OrthographicCamera>(handle))) {
			ecs.add<OrthographicCamera>(handle, device, u32_2{480, 480});
		}
		if (ImGui::MenuItem("Perspective Camera", nullptr, nullptr, !ecs.has<PerspectiveCamera>(handle))) {
			ecs.add<PerspectiveCamera>(handle, device, u32_2{480, 480});
		}
		if (ImGui::MenuItem("Directional Light", nullptr, nullptr, !ecs.has<DirectionalLight>(handle))) {
			ecs.add<DirectionalLight>(handle);
		}
		if (ImGui::MenuItem("Point Light", nullptr, nullptr, !ecs.has<PointLight>(handle))) {
			ecs.add<PointLight>(handle);
		}
		if (ImGui::MenuItem("Spot Light", nullptr, nullptr, !ecs.has<SpotLight>(handle))) {
			ecs.add<SpotLight>(handle);
		}

		if (ImGui::BeginMenu("Model", !ecs.has<Model>(handle))) {
			if (ImGui::MenuItem("From file")) {
				if (auto file = OpenFileDialog(); fs::exists(file)) {
					ModelConfig<VertexPositionNormalTexture> config;
					auto output = importer::ImportModel(resource_mgr, file, config);
					auto bp = resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(output.name), output, config);
					if (valid_entity) {
						scene.importModel(handle, device, bp);
					}
				}
				else {
					Logger::log(LogLevel::err, "Failed to open file dialog");
				}
			}

			if (ImGui::BeginMenu("Geometric Shape")) {
				new_model_menu.drawMenu();
				ImGui::EndMenu();
			}

			ImGui::EndMenu(); //Model
		}

		// List user components
		for (const auto& [idx, component] : user_components) {
			if (component.adder) {
				if (ImGui::MenuItem(component.name.c_str(), nullptr, nullptr, !component.getter(ecs, handle))) {
					component.adder(ecs, handle);
				}
			}
		}

		ImGui::EndMenu(); //Add Component
	}
}


template<typename T, typename... ArgsT>
void EntityDetailsWindow::drawComponentNode(ecs::ECS& ecs,
                                            gsl::czstring text,
                                            T& component,
                                            ArgsT&&... args) {

	bool dont_delete = true;
	ImGui::PushID(&component);
	if (ImGui::CollapsingHeader(text, &dont_delete)) {
		DrawComponentState(component);
		drawDetails(component, std::forward<ArgsT>(args)...);
	}
	ImGui::PopID();
	if (!dont_delete) {
		ecs.remove(component.getOwner(), component);
	}
}


void EntityDetailsWindow::drawUserComponentNode(ecs::ECS& ecs,
                                                gsl::czstring text,
                                                ecs::Component& component,
                                                const UserComponent::details_func& draw_func) {

	bool dont_delete = true;
	ImGui::PushID(&component);
	if (ImGui::CollapsingHeader(text, &dont_delete)) {
		DrawComponentState(component);
		if (draw_func) {
			draw_func(component);
		}
	}
	ImGui::PopID();
	if (!dont_delete) {
		ecs.remove(component.getOwner(), component);
	}
}


template<>
void EntityDetailsWindow::drawDetails(Transform& transform) {

	f32_3 position;
	f32_3 rotation;
	f32_3 scale;
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
	static constexpr gsl::czstring render_mode_names[] = {
	    "Forward",
	    "Forward+",
	    "Deferred",
	    "False Color",
	};
	static constexpr RenderMode render_modes[] = {
	    RenderMode::Forward,
	    RenderMode::ForwardPlus,
	    RenderMode::Deferred,
		RenderMode::FalseColor,
	};

	auto render_mode = static_cast<int>(settings.getRenderMode());
	if (ImGui::Combo("Render Mode", &render_mode, render_mode_names, static_cast<int>(std::size(render_mode_names))))
		settings.setRenderMode(static_cast<RenderMode>(render_mode));


	//----------------------------------------------------------------------------------
	// BRDF
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring brdf_names[] = {
	    "Lambert",
	    "Blinn-Phong",
	    "Microfacet",
	};
	static constexpr BRDF brdfs[] = {
	    BRDF::Lambert,
	    BRDF::BlinnPhong,
	    BRDF::Microfacet,
	};

	if (settings.getRenderMode() != RenderMode::FalseColor) {
		auto brdf = static_cast<int>(settings.getBRDF());
		if (ImGui::Combo("BRDF", &brdf, brdf_names, static_cast<int>(std::size(brdf_names))))
			settings.setBRDF(static_cast<BRDF>(brdf));
	}


	//----------------------------------------------------------------------------------
	// FalseColor
	//----------------------------------------------------------------------------------
	static constexpr gsl::czstring false_color_names[] = {
	    "Fullbright",
	    "Texture Coord",
	    "Material Params",
	    "Metalness",
	    "Roughness",
	    "Normal",
	    "Depth",
	};
	static constexpr FalseColor false_colors[] = {
	    FalseColor::Fullbright,
	    FalseColor::TextureCoord,
		FalseColor::MaterialParams,
		FalseColor::Metalness,
		FalseColor::Roughness,
	    FalseColor::Normal,
	    FalseColor::Depth,
	};

	if (settings.getRenderMode() == RenderMode::FalseColor) {
		auto false_color = static_cast<int>(settings.getFalseColorMode());
		if (ImGui::Combo("False Color Mode", &false_color, false_color_names, static_cast<int>(std::size(false_color_names))))
			settings.setFalseColorMode(static_cast<FalseColor>(false_color));
	}


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
	u32_2 size = vp.getSize();
	u32_2 pos = vp.getTopLeft();

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
	f32_2 depth = camera.getZDepth();
	if (ImGui::DragFloat2("Depth Range", depth.data(), 1.0f, 0.01f, FLT_MAX)) {
		camera.setZDepth(depth);
	}


	DrawCameraSettings(camera.getSettings());
}


template<>
void EntityDetailsWindow::drawDetails(OrthographicCamera& camera) {

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
	u32_2 size = vp.getSize();
	u32_2 pos = vp.getTopLeft();

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
	f32_2 depth = camera.getZDepth();
	if (ImGui::DragFloat2("Depth Range", depth.data(), 1.0f, 0.01f, FLT_MAX))
		camera.setZDepth(depth);


	DrawCameraSettings(camera.getSettings());
}


template<>
void EntityDetailsWindow::drawDetails(Text& text) {

	std::string str = WstrToStr(text.getText());

	if (ImGui::InputTextMultiline("Text", &str)) {
		text.setText(StrToWstr(str));
	}
}


template<>
void EntityDetailsWindow::drawDetails(Model& model, ResourceMgr& resource_mgr) {

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
	const char* shader_title_preview = mat.shader ? mat.shader->getGUID().c_str() : "None";

	drawResourceMapComboBox<PixelShader>("Override Shader", shader_title_preview, resource_mgr, mat.shader);

	ImGui::Separator();

	//----------------------------------------------------------------------------------
	// Shadows
	//----------------------------------------------------------------------------------
	bool shadows = model.castsShadows();
	if (ImGui::Checkbox("Casts Shadows", &shadows))
		model.setShadows(shadows);
}


template<>
void EntityDetailsWindow::drawResourceMapComboBox(const char* name,
                                                  const char* preview_text,
                                                  ResourceMgr& resource_mgr,
                                                  std::shared_ptr<Texture>& selection_receiver) {

	// If the combo box is opened, a pointer to the current texture will be
	// kept in order to prevent it from potentially expiring before ImGui draws it.
	static std::shared_ptr<Texture> last;

	//----------------------------------------------------------------------------------
	// Draw the list of textures with a preview
	//----------------------------------------------------------------------------------
	if (ImGui::BeginCombo("", preview_text)) {
		last = selection_receiver;

		const auto& resource_map = resource_mgr.getResourceMap<Texture>();
		res_map_names.clear();

		if (ImGui::Selectable("None")) {
			selection_receiver = nullptr;
		}
		for (const auto& [key, sharedptr] : resource_map) {
			if (!sharedptr) continue;

			res_map_names.push_back(std::ref(sharedptr->getGUID()));

			ImGui::Image(reinterpret_cast<ImTextureID>(sharedptr->get()), {15, 15});
			ImGui::SameLine();
			if (ImGui::Selectable(res_map_names.back().get().c_str())) {
				selection_receiver = sharedptr;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine();

	
	//----------------------------------------------------------------------------------
	// Draw the "new texture" button and popups
	//----------------------------------------------------------------------------------
	bool new_file_texture_popup  = false;
	bool new_color_texture_popup = false;

	ImGui::PushID(name);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (ImGui::GetStyle().ItemSpacing.x * 0.5f));
	if (ImGui::Button("+")) {
		ImGui::OpenPopup("NewTextureType");
	}
	if (ImGui::BeginPopup("NewTextureType")) {
		if (ImGui::MenuItem("File")) {
			new_file_texture_popup = true;
		}
		if (ImGui::MenuItem("Color")) {
			new_color_texture_popup = true;
		}
		ImGui::EndPopup();
	}

	if (new_file_texture_popup) {
		ImGui::OpenPopup("New File Texture");
	}
	if (new_color_texture_popup) {
		ImGui::OpenPopup("New Color Texture");
	}

	if (ImGui::BeginPopupModal("New File Texture", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("Path:");
		ImGui::InputText("", &new_file_texture_path);

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::Button("...")) {
			new_file_texture_path = OpenFileDialog().string();
		}

		ImGui::Spacing();

		if (ImGui::Button("Load")) {
			selection_receiver = resource_mgr.getOrCreate<Texture>(StrToWstr(new_file_texture_path));
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Color Texture", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::ColorEdit4("Color", new_color_texture_color.data());

		ImGui::Spacing();

		if (ImGui::Button("Create")) {
			selection_receiver = TextureFactory::CreateColorTexture(resource_mgr, new_color_texture_color);
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::PopID();

	
	//----------------------------------------------------------------------------------
	// Draw the combo box name
	//----------------------------------------------------------------------------------
	ImGui::SameLine();
	ImGui::Text(name);
}


template<typename ResourceT>
void EntityDetailsWindow::drawResourceMapComboBox(const char* name,
                                                  const char* preview_text,
                                                  ResourceMgr& resource_mgr,
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

			if (ImGui::Selectable(res_map_names.back().get().c_str())) {
				selection_receiver = sharedptr;
			}
		}
		ImGui::EndCombo();
	}
}


template<>
void EntityDetailsWindow::drawDetails(AmbientLight& light) {

	auto color = light.getColor();
	if (ImGui::ColorEdit3("Ambient Color", color.data()))
		light.setColor(color);
}


template<>
void EntityDetailsWindow::drawDetails(DirectionalLight& light) {

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

#pragma once

#include "datatypes/datatypes.h"
#include "io/io.h"
#include "log/log.h"
#include "imgui_impl_dx11.h"
#include "buffer/buffers.h"
#include "ecs.h"
#include "resource/model/blueprint_factory.h"
#include "entities/entities.h"
#include "components/components.h"

class Scene;
class Engine;

class UserInterface {
public:
	UserInterface() = default;
	~UserInterface() = default;

	void draw(const Engine& engine);


private:
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


private:
	void drawMenu(ID3D11Device& device, ECS& ecs_engine, ResourceMgr& resource_mgr, Scene& scene, ModelType& add_model_popup) const;
	void drawObjectLists(ECS& ecs_engine, Scene& scene);


	template<typename T>
	void drawDetailsPanel(T& component) {
		ImGui::Begin("Properties");
		drawDetails(component);
		ImGui::End();
	}

	void drawDetails(Scene& scene) const;
	void drawDetails(Transform& transform) const;
	void drawDetails(PerspectiveCamera& camera) const;
	void drawDetails(OrthographicCamera& camera) const;
	void drawDetails(CameraMovement& movement) const;
	void drawDetails(Model& model) const;
	void drawDetails(ModelChild& child) const;
	void drawDetails(DirectionalLight& light) const;
	void drawDetails(PointLight& light) const;
	void drawDetails(SpotLight& light) const;

	void procNewModelPopups(ID3D11Device& device, ECS& ecs_engine, ResourceMgr& resource_mgr, handle64 entity, ModelType type) const;
};
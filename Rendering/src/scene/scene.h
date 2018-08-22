#pragma once

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"
#include "systems/systems.h"
#include "scene/text/text.h"

class Engine;

class Scene {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Scene(const Scene& scene) = delete;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Scene() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Scene& operator=(const Scene& scene) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions - Misc
	//----------------------------------------------------------------------------------

	// Load the scene contents
	virtual void load(const Engine& engine) = 0;

	// Update the scene
	virtual void tick(Engine& engine) = 0;

	// Handle a window resize event
	virtual void onResize(vec2_u32 size) {
		ecs->forEach<PerspectiveCamera>([&size](auto& camera) {
			camera.getViewport().setSize(size);
		});
		ecs->forEach<OrthographicCamera>([&size](auto& camera) {
			camera.getViewport().setSize(size);
		});
	}

	[[nodiscard]]
	const std::string& getName() const {
		return name;
	}

	[[nodiscard]]
	ECS& getECS() const {
		return *ecs;
	}

	[[nodiscard]]
	const std::map<std::string, Text>& getTexts() const {
		return texts;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	// Add an entity to this scene
	template<typename EntityT, typename... ArgsT>
	handle64 addEntity(ArgsT&&... args) {
		auto handle = ecs->createEntity<EntityT>(std::forward<ArgsT>(args)...);
		entities.push_back(handle);
		return handle;
	}

	// Remove an entity from this scene
	void removeEntity(handle64 entity) {
		entities.erase(std::remove(std::begin(entities), std::end(entities), entity),
		               std::end(entities));
		ecs->destroyEntity(entity);
	}

	[[nodiscard]]
	const std::vector<handle64>& getEntities() const {
		return entities;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Import Model
	//----------------------------------------------------------------------------------

	// Create a new entity and add models described by a model blueprint
	handle64 importModel(ID3D11Device& device, shared_ptr<ModelBlueprint>& blueprint) {
		handle64 out = addEntity<WorldObject<>>();
		importModel(out, device, blueprint);
		return out;
	}

	// Add models described by a model blueprint to an existing entity
	void importModel(handle64 entity, ID3D11Device& device, shared_ptr<ModelBlueprint>& blueprint) {
		const auto& materials = blueprint->getMaterials();
		blueprint->forEachPart([&](ModelPart& part) {
			ecs->addComponent<Model>(entity, device, blueprint->mesh, part, materials[part.material_index]);
		});
	}



protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Scene()
		: name("Scene")
		, ecs(std::make_unique<ECS>()) {
	}

	Scene(std::string name)
		: name(std::move(name))
		, ecs(std::make_unique<ECS>()) {
	}

	Scene(Scene&& scene) = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Scene& operator=(Scene&& scene) = default;



protected:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The name of the scene
	std::string name;

	// The ECS engine for this scene
	unique_ptr<ECS> ecs;

	// Entitites that currently exist in this scene
	std::vector<handle64> entities;

	// Text objects in this scene
	std::map<std::string, Text> texts;
};
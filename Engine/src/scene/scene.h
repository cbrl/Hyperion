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
	virtual void tick(const Engine& engine) = 0;

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
	const string& getName() const {
		return name;
	}

	[[nodiscard]]
	ECS& getECS() const {
		return *ecs;
	}

	[[nodiscard]]
	const map<string, Text>& getTexts() const {
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
	const vector<handle64>& getEntities() const {
		return entities;
	}



protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Scene()
		: name("Scene")
		, ecs(make_unique<ECS>()) {
	}

	Scene(string name)
		: name(std::move(name))
		, ecs(make_unique<ECS>()) {
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
	string name;

	// The ECS engine for this scene
	unique_ptr<ECS> ecs;

	// Entitites that currently exist in this scene
	vector<handle64> entities;

	// Text objects in this scene
	map<string, Text> texts;
};
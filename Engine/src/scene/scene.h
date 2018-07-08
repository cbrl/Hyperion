#pragma once

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"
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

	// Unload the scene contents
	void unload(const Engine& engine);

	// Update the scene
	virtual void tick(const Engine& engine) = 0;

	[[nodiscard]]
	const string& getName() const {
		return name;
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
	handle64 addEntity(ECS& ecs_engine, ArgsT&&... args) {
		auto handle = ecs_engine.createEntity<EntityT>(std::forward<ArgsT>(args)...);
		entities.push_back(handle);
		return handle;
	}

	// Remove an entity from this scene
	void removeEntity(ECS& ecs_engine, handle64 entity) {
		entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		ecs_engine.destroyEntity(entity);
	}

	[[nodiscard]]
	const vector<handle64>& getEntities() const {
		return entities;
	}



protected:
	Scene()
		: name("Scene") {
	}

	Scene(string name)
		: name(std::move(name)) {
	}

	Scene(Scene&& scene) = default;

	Scene& operator=(Scene&& scene) = default;


protected:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	string name;
	map<string, Text> texts;
	vector<handle64> entities;
};
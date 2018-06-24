#pragma once

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"
#include "scene/text/text.h"

class Engine;

class Scene {
public:
	Scene(const Scene& scene) = delete;
	virtual ~Scene() = default;

	virtual void load(const Engine& engine) = 0;

	void unload(const Engine& engine);

	// Update the scene
	virtual void tick(const Engine& engine) = 0;

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


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const string& getName() const {
		return name;
	}

	[[nodiscard]]
	const map<string, Text>& getTexts() const {
		return texts;
	}

	[[nodiscard]]
	const vector<handle64>& getEntities() const {
		return entities;
	}


protected:
	Scene()
		: name("Scene") {
	}

	Scene(string&& name)
		: name(std::move(name)) {
	}

	Scene(Scene&& scene) = default;


protected:
	string name;
	map<string, Text> texts;
	vector<handle64> entities;
};
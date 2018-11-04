#pragma once

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"
#include "systems/systems.h"

#include "scene/entities/world_object.h"
#include "scene/components/core_components.h"
#include "scene/systems/core_systems.h"

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
	// Member Functions - Execution
	//----------------------------------------------------------------------------------

	// Load the scene contents
	virtual void load(const Engine& engine) = 0;

	// Update the scene
	virtual void tick(Engine& engine) = 0;


	//----------------------------------------------------------------------------------
	// Member Functions - Window Resize
	//----------------------------------------------------------------------------------

	// Handle a window resize event
	void onResize(vec2_u32 size);


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::string& getName() const {
		return name;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	// Add an entity to this scene
	template<typename EntityT = WorldObject, typename... ArgsT>
	EntityPtr addEntity(ArgsT&&... args) {
		auto entity = ecs->createEntity<EntityT>(std::forward<ArgsT>(args)...);
		entities.push_back(entity);
		return entity;
	}

	// Remove an entity from this scene
	void removeEntity(EntityPtr entity) {
		if (entity) {
			ecs->destroyEntity(entity.getHandle());

			if (auto parent = entity->getParent()) {
				parent->removeChild(entity);
			}
		}
		entities.erase(std::remove(std::begin(entities), std::end(entities), entity),
		               std::end(entities));
	}

	[[nodiscard]]
	const std::vector<EntityPtr>& getEntities() const {
		return entities;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	template<typename T, typename ActionT>
	void forEach(ActionT&& act) {
		ecs->forEach<T>(act);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Import Model
	//----------------------------------------------------------------------------------

	// Create a new entity and add models described by a model blueprint
	EntityPtr importModel(ID3D11Device& device, const shared_ptr<ModelBlueprint>& blueprint);

	// Add models described by a model blueprint to an existing entity
	void importModel(EntityPtr entity, ID3D11Device& device, const shared_ptr<ModelBlueprint>& blueprint);


protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Scene()
		: name("Scene")
		, ecs(std::make_unique<ECS>()) {
		addCriticalSystems();
	}

	Scene(std::string name)
		: name(std::move(name))
		, ecs(std::make_unique<ECS>()) {
		addCriticalSystems();
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
	std::vector<EntityPtr> entities;


private:
	//----------------------------------------------------------------------------------
	// Member Functions - Add Systems
	//----------------------------------------------------------------------------------

	// Add systems required for normal operation to the ECS
	void addCriticalSystems();
};
#pragma once

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"
#include "systems/systems.h"

#include "scene/entities/core_entities.h"
#include "scene/components/core_components.h"
#include "scene/systems/core_systems.h"

using namespace EntityTemplates;
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
	template<typename TemplateT = WorldObject, typename... ArgsT>
	EntityPtr addEntity(ArgsT&&... args) {
		auto ptr = ecs->createEntity();
		entities.push_back(ptr);
		TemplateT::applyTemplate(*ptr, std::forward<ArgsT>(args)...);
		return ptr;
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
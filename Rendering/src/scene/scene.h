#pragma once

#include "ecs.h"
#include "scene/entities/core_entities.h"
#include "scene/components/core_components.h"
#include "scene/systems/core_systems.h"
#include "scene/events/core_events.h"

class Engine;

namespace render {

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
	void load(Engine& engine);

	// Update the scene
	void tick(Engine& engine);


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
	template<typename TemplateT = EntityTemplates::WorldObjectT, typename... ArgsT>
	ecs::EntityPtr addEntity(ArgsT&&... args);

	// Remove an entity from this scene
	void removeEntity(ecs::EntityPtr entity);

	[[nodiscard]]
	const std::vector<ecs::EntityPtr>& getEntities() const {
		return entities;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	SystemT& addSystem(ArgsT&&... args);

	void removeSystem(ecs::ISystem& system);

	template <typename SystemT>
	void removeSystem();


	//----------------------------------------------------------------------------------
	// Member Functions - Events
	//----------------------------------------------------------------------------------
	template<typename EventT, typename... ArgsT>
	void sendEvent(ArgsT&&... args);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(const std::function<void(ecs::Entity&)>& act);

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(const std::function<void(const ecs::Entity&)>& act) const;

	// Do something with each component of type ComponentT
	template<typename ComponentT>
	void forEach(const std::function<void(ComponentT&)>& act);

	// Do something with each component of type ComponentT
	template<typename ComponentT>
	void forEach(const std::function<void(const ComponentT&)>& act) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Import Model
	//----------------------------------------------------------------------------------

	// Import a model blueprint under a new entity
	[[nodiscard]]
	ecs::EntityPtr importModel(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint);

	// Import a model blueprint under an existing entity
	void importModel(const ecs::EntityPtr& ptr, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint);


protected:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Scene()
		: name("Scene")
	    , ecs(std::make_unique<ecs::ECS>()) {
	}

	Scene(std::string name)
		: name(std::move(name))
	    , ecs(std::make_unique<ecs::ECS>()) {
	}

	Scene(Scene&& scene) = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Scene& operator=(Scene&& scene) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Overrided by the derived class and called by Scene::load()
	virtual void initialize(Engine& engine) = 0;

	// Update the scene per tick. Overrided by the derived class and called by Scene::tick()
	virtual void update(Engine& engine) = 0;


private:

	// Add systems required for normal operation to the ECS
	void addCoreSystems(const Engine& engine);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The name of the scene
	std::string name;

	// The ECS engine for this scene
	std::unique_ptr<ecs::ECS> ecs;

	// Entitites that currently exist in this scene
	std::vector<ecs::EntityPtr> entities;
};

} //namespace render

#include "scene.tpp"
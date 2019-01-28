#pragma once

#include "ecs.h"
#include "entities/entities.h"
#include "components/components.h"
#include "systems/systems.h"

#include "scene/entities/core_entities.h"
#include "scene/components/core_components.h"
#include "scene/events/core_events.h"

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
	void load(const Engine& engine);

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
	template<typename TemplateT = WorldObjectT, typename... ArgsT>
	EntityPtr addEntity(ArgsT&&... args);

	// Remove an entity from this scene
	void removeEntity(EntityPtr entity);

	[[nodiscard]]
	const std::vector<EntityPtr>& getEntities() const {
		return entities;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	SystemT& addSystem(ArgsT&&... args);

	void removeSystem(ISystem& system);

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
	template<typename T, typename ActionT>
	void forEach(ActionT&& act);


	//----------------------------------------------------------------------------------
	// Member Functions - Import Model
	//----------------------------------------------------------------------------------

	// Import a model blueprint under a new entity
	[[nodiscard]]
	EntityPtr importModel(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint);

	// Import a model blueprint under an existing entity
	void importModel(const EntityPtr& ptr, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint);


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


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Overrided by the derived class and called by Scene::load()
	virtual void initialize(const Engine& engine) = 0;

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
	std::unique_ptr<ECS> ecs;

	// Entitites that currently exist in this scene
	std::vector<EntityPtr> entities;
};

#include "scene.tpp"

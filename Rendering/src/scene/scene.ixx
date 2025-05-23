module;

#include <memory>
#include <string>
#include <utility>

#include "memory/handle/handle.h"

#include "directx/d3d11.h"

export module rendering:scene;

export import ecs;
import :entities.core_entities;
import :model_blueprint;

export namespace render {

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
	void load(Engine& engine);

	// Update the scene
	void tick(Engine& engine);


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	const std::string& getName() const noexcept {
		return name;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - ECS
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	ecs::ECS& getECS() noexcept {
		return ecs;
	}

	[[nodiscard]]
	const ecs::ECS& getECS() const noexcept {
		return ecs;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	// Add an entity to this scene and apply the given template to it
	template<typename TemplateT = EntityTemplates::WorldObjectT, typename... ArgsT>
	handle64 createEntity(ArgsT&& ... args) {
		auto handle = ecs.create();

		TemplateT{}(ecs, handle, std::forward<ArgsT>(args)...);
		return handle;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Import Model
	//----------------------------------------------------------------------------------

	// Import a model blueprint under a new entity
	[[nodiscard]]
	handle64 importModel(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint);

	// Import a model blueprint under an existing entity
	void importModel(handle64 handle, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint);


protected:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Scene() : name("Scene") {
	}

	Scene(std::string name) : name(std::move(name)) {
	}

	Scene(Scene&& scene) = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Scene& operator=(Scene&& scene) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Overridden by the derived class and called by Scene::load()
	virtual void initialize(Engine& engine) = 0;

	// Update the scene per tick. Overridden by the derived class and called by Scene::tick()
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
	ecs::ECS ecs;
};

} //namespace render

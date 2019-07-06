#pragma once

#include "selectable_tree.h"
#include "memory/handle/handle.h"

namespace render {
	class Scene;
}

namespace ecs {
	class ECS;
}

class SceneTree final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SceneTree() = default;
	SceneTree(const SceneTree&) = default;
	SceneTree(SceneTree&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SceneTree() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SceneTree& operator=(const SceneTree&) = default;
	SceneTree& operator=(SceneTree&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void draw(render::Scene& scene);

	[[nodiscard]]
	handle64 getSelectedEntity() const noexcept;

	[[nodiscard]]
	void setSelectedEntity(handle64 entity) noexcept;

private:

	void drawTree(render::Scene& scene);
	void drawEntityNode(ecs::ECS& ecs, handle64 entity);

	void drawMenuBar(render::Scene& scene);
	void drawEntityMenu(render::Scene& scene);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	SelectableTree<handle64::value_type> scene_tree;
};
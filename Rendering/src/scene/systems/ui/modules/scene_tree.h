#pragma once

#include "selectable_tree.h"

namespace render { class Scene; }
namespace ecs { class EntityPtr; }

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
	ecs::EntityPtr getSelectedEntity() const noexcept;

private:

	void drawTree(render::Scene& scene);
	void drawEntityNode(ecs::EntityPtr entity_ptr);

	void drawMenuBar(render::Scene& scene);
	void drawEntityMenu(render::Scene& scene);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	SelectableTree<ecs::EntityPtr> scene_tree;
};
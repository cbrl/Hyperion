#pragma once

#include "selectable_tree.h"

class Scene;
class ecs::EntityPtr;

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
	void draw(Scene& scene);

	[[nodiscard]]
	ecs::EntityPtr getSelectedEntity() const noexcept;

private:

	void drawTree(Scene& scene);
	void drawEntityNode(ecs::EntityPtr entity_ptr);

	void drawMenuBar(Scene& scene);
	void drawEntityMenu(Scene& scene);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	SelectableTree<ecs::EntityPtr> scene_tree;
};
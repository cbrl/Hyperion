#pragma once

#include "selectable_tree.h"

class Scene;
class EntityPtr;

class SceneTree final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SceneTree();
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
	EntityPtr getSelectedEntity() const noexcept;

private:

	void drawTree(Scene& scene);
	void drawEntityNode(EntityPtr entity_ptr);

	void drawMenuBar(Scene& scene);
	void drawEntityMenu(Scene& scene);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	SelectableTree<EntityPtr> scene_tree;
};
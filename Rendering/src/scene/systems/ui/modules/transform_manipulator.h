#pragma once

#include "imgui.h"
#include "imgui_addons/ImGuizmo/ImGuizmo.h"

class Engine;
class Input;
class Transform;
namespace ecs { class EntityPtr; }

class TransformManipulator {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TransformManipulator() = default;
	TransformManipulator(const TransformManipulator&) = default;
	TransformManipulator(TransformManipulator&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~TransformManipulator() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TransformManipulator& operator=(const TransformManipulator&) = default;
	TransformManipulator& operator=(TransformManipulator&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void draw(Engine& engine, ecs::EntityPtr selected_entity);

private:

	template<typename CameraT>
	void drawTransformManipulator(Transform& transform, CameraT& camera, Input& input);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE mode = ImGuizmo::WORLD;
};
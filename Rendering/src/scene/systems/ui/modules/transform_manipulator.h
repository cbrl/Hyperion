#pragma once

#include "imgui.h"
#include "imgui_addons/ImGuizmo/ImGuizmo.h"
#include "datatypes/container_types.h"

class Engine;
class Input;
class KeyConfig;
class Transform;
namespace ecs { class EntityPtr; }

class TransformManipulator {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TransformManipulator(Input& input, KeyConfig& key_config);
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
	void drawTransformManipulator(Transform& transform, CameraT& camera);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<Input> input;
	std::reference_wrapper<KeyConfig> key_config;

	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE mode = ImGuizmo::WORLD;
};
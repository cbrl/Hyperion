#pragma once

#include "datatypes/datatypes.h"
#include "scene/scene.h"
#include "resource/model/blueprint_factory.h"


class TestScene final : public render::Scene {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TestScene();
	TestScene(const TestScene&) = delete;
	TestScene(TestScene&&) = default;

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~TestScene() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TestScene& operator=(const TestScene&) = delete;
	TestScene& operator=(TestScene&&) = default;
	
protected:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void initialize(Engine& engine) override;
	void update(Engine& engine) override;

private:

	void addUserComponentsToUI(render::systems::UserInterface& ui);

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	handle64 scene_name_text;
};

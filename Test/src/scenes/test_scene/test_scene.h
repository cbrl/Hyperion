#pragma once

#include "datatypes/datatypes.h"
#include "scene/scene.h"
#include "resource/model/blueprint_factory.h"


class TestScene final : public Scene {
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

	void addComponentDetailRenderes(UserInterface& ui);

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	EntityPtr scene_name_text;
};

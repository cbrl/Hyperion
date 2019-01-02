#pragma once

#include "datatypes/datatypes.h"
#include "scene/scene.h"
#include "resource/model/blueprint_factory.h"


class TestScene final : public Scene {
public:
	TestScene();
	~TestScene() = default;

	void initialize(const Engine& engine);

	void update(Engine& engine) override final;


private:
	EntityPtr text_cpu;
	EntityPtr text_ram;
	EntityPtr text_mouse;
};

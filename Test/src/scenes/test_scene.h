#pragma once

#include "datatypes/datatypes.h"
#include "scene/scene.h"
#include "resource/model/blueprint_factory.h"


class TestScene final : public Scene {
public:
	TestScene();

	~TestScene() = default;

	void load(const Engine& engine) override;

	void tick(Engine& engine) override;


private:
	EntityPtr text_fps;
	EntityPtr text_frame_time;
	EntityPtr text_cpu;
	EntityPtr text_ram;
	EntityPtr text_mouse;
};

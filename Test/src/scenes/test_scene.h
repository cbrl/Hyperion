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
	handle64 text_fps;
	handle64 text_frame_time;
	handle64 text_cpu;
	handle64 text_ram;
	handle64 text_mouse;
};

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
};

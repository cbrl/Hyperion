#pragma once

#include "datatypes/datatypes.h"
#include "scene/scene.h"
#include "resource/model/blueprint_factory.h"


class TestScene final : public Scene {
public:
	TestScene();
	~TestScene() = default;


protected:
	void initialize(Engine& engine) override;

	void update(Engine& engine) override;


private:
	EntityPtr scene_name_text;
};

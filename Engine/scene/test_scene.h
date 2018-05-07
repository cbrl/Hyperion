#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "scene\scene.h"
#include "resource\model\blueprint_factory.h"


class TestScene final : public Scene {

	public:
		TestScene(const Engine& engine);

		~TestScene() = default;

		void Init(const Engine& engine);

		void Tick(const Engine& engine);
};
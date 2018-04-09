#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "scene\scene.h"


class TestScene final : public Scene {

	public:
		TestScene(const System& system);

		~TestScene() = default;

		void Init(const System& system);

		void Tick(const System& system);
};
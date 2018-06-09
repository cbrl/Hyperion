#pragma once

#include "datatypes/datatypes.h"
#include "input/input.h"
#include "rendering/buffer/buffers.h"
#include "resource/resource_mgr.h"
#include "scene/text/text.h"
//#include "scene\ui\user_interface.h"
#include "ecs_data/entities/entities.h"
#include "ecs_data/components/components.h"


class Engine;


class Scene {
public:
	virtual ~Scene() = default;

	// Update the scene
	virtual void tick(const Engine& engine) = 0;

	// Draw the UI
	//void DrawUI(Engine& engine) { ui.Draw(engine); }


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

	auto& getName() const { return name; }
	auto& getFog() const { return fog; }
	auto& getTexts() const;


protected:
	Scene() : name("Scene") {
	}

	Scene(string&& name) : name(std::move(name)) {
	}

	virtual void init(const Engine& engine) = 0;


protected:
	string name;
	//UserInterface     ui;
	Fog fog;
	map<string, Text> texts;
	vector<handle64> entities;
};


inline auto& Scene::getTexts() const { return texts; }

#pragma once

#include "util\datatypes\datatypes.h"
#include "input\input.h"
#include "rendering\buffer\buffers.h"
#include "resource\resource_mgr.h"
#include "scene\text\text.h"
//#include "scene\ui\user_interface.h"
#include "ecs_data\entities\entities.h"
#include "ecs_data\components\components.h"


class Engine;


class Scene {
	public:
		~Scene() = default;

		// Update the scene
		virtual void Tick(const Engine& engine) = 0;

		// Draw the UI
		//void DrawUI(Engine& engine) { ui.Draw(engine); }


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		auto& GetName()  const { return name; }
		auto& GetFog()   const { return fog; }
		auto& GetTexts() const { return texts; }


	protected:
		Scene() : name("Scene") {}

		Scene(string&& name) : name(std::move(name)) {}

		virtual void Init(const Engine& engine) = 0;


	protected:
		string            name;
		//UserInterface     ui;
		Fog               fog;
		map<string, Text> texts;
		vector<Handle64>  entities;
};


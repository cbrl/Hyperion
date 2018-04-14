#pragma once

#include "util\datatypes\datatypes.h"
#include "input\input.h"
#include "camera\player_camera.h"
#include "rendering\buffer\buffers.h"
#include "resource\resource_mgr.h"
#include "resource\model\model.h"
#include "resource\text\text.h"
#include "scene\ui\user_interface.h"

class System;

class Scene {
	public:
		~Scene() = default;

		// Enable or disable input
		void InputEnabled(bool enable) { enable_input = enable; }

		// Update the scene
		virtual void Tick(const System& system) = 0;

		// Draw the UI
		void DrawUI(System& system) { ui.Draw(system); }


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		auto& GetCamera() const      { return *camera; }
		auto& GetFog()               { return fog; }
		auto& GetDirectionalLights() { return directional_lights; }
		auto& GetPointLights()       { return point_lights; }
		auto& GetSpotLights()        { return spot_lights; }
		auto& GetModels()            { return models; }


	protected:
		Scene() : enable_input(true) {}
		virtual void Init(const System& system) = 0;


	protected:
		bool                     enable_input;

		unique_ptr<PlayerCamera> camera;
		Fog                      fog;
		vector<Model>            models;
		vector<PointLight>       point_lights;
		vector<DirectionalLight> directional_lights;
		vector<SpotLight>        spot_lights;
		map<string, Text>        texts;

		UserInterface            ui;


	public:
		template<typename ElementT, typename ActionT>
		void ForEach(ActionT act) {
			if constexpr (is_same_v<Model, ElementT>) {
				for (auto& e : models) {
					act(e);
				}
			}

			if constexpr (is_same_v<Text, ElementT>) {
				for (auto& e : texts) {
					act(e.second);
				}
			}

			if constexpr (is_same_v<DirectionalLight, ElementT>) {
				for (auto& e : directional_lights) {
					act(e);
				}
			}

			if constexpr (is_same_v<PointLight, ElementT>) {
				for (auto& e : point_lights) {
					act(e);
				}
			}

			if constexpr (is_same_v<SpotLight, ElementT>) {
				for (auto& e : spot_lights) {
					act(e);
				}
			}
		}
};


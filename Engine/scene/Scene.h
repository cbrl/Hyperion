#pragma once

#include "util\datatypes\datatypes.h"
#include "input\input.h"
#include "rendering\buffer\buffers.h"
#include "resource\resource_mgr.h"
#include "scene\text\text.h"
#include "scene\camera\skybox\skybox.h"
#include "scene\camera\player_camera.h"
#include "scene\ui\user_interface.h"
#include "ecs_data\entities\entities.h"
#include "ecs_data\components\components.h"


class System;



enum class BlendStates {
	Default          = 0,
	Opaque           = 1,
	AlphaBlend       = 2,
	Additive         = 3,
	NonPremultiplied = 4
};
enum class DepthStates {
	Default      = 0,
	DepthNone    = 1,
	DepthDefault = 2,
	DepthRead    = 3
};
enum class RasterStates {
	Default              = 0,
	CullNone             = 1,
	CullClockwise        = 2,
	CullCounterClockwise = 3,
	Wireframe            = 4
};

struct RenderStates {
	BlendStates  blend_state  = BlendStates::Default;
	DepthStates  depth_state  = DepthStates::Default;
	RasterStates raster_state = RasterStates::Default;
};



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

		auto& GetName()         const { return name; }
		auto& GetCamera()       const { return *camera; }
		auto& GetFog()                { return fog; }
		auto& GetDirectionalLights()  { return directional_lights; }
		auto& GetPointLights()        { return point_lights; }
		auto& GetSpotLights()         { return spot_lights; }
		auto& GetModels()             { return models; }
		auto& GetRenderStates() const { return render_states; }


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		void SetBlendState(BlendStates state)   { render_states.blend_state  = state; }
		void SetDepthState(DepthStates state)   { render_states.depth_state  = state; }
		void SetRasterState(RasterStates state) { render_states.raster_state = state; }


	protected:
		Scene() : name("Scene"), enable_input(true) {}

		Scene(const string& name) : name(name), enable_input(true) {}

		virtual void Init(const System& system) = 0;


	protected:
		string                         name;

		bool                           enable_input;
		RenderStates                   render_states;
		UserInterface                  ui;

		unique_ptr<PlayerCamera>       camera;
		Fog                            fog;
		vector<Model>                  models;
		vector<PointLightBuffer>       point_lights;
		vector<DirectionalLightBuffer> directional_lights;
		vector<SpotLightBuffer>        spot_lights;
		map<string, Text>              texts;

		vector<Handle64>               entities;


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

			if constexpr (is_same_v<DirectionalLightBuffer, ElementT>) {
				for (auto& e : directional_lights) {
					act(e);
				}
			}

			if constexpr (is_same_v<PointLightBuffer, ElementT>) {
				for (auto& e : point_lights) {
					act(e);
				}
			}

			if constexpr (is_same_v<SpotLightBuffer, ElementT>) {
				for (auto& e : spot_lights) {
					act(e);
				}
			}
		}


		template<typename ActionT>
		void ForEachEntity(ActionT act) {
			for (Handle64 entity : entities) {
				act(entity);
			}
		}
};


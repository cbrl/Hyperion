#pragma once

#include "util\datatypes\datatypes.h"
#include "input\input.h"
#include "camera\camera.h"
#include "rendering\buffer\buffers.h"
#include "resource\resource_mgr.h"
#include "resource\model\model.h"
#include "resource\skybox\skybox.h"
#include "text\text.h"


class Scene {
	public:
		~Scene() = default;

		// Enable or disable input
		void InputEnabled(bool enable) { enable_input = enable; }

		// Update the scene
		virtual void Tick(float delta_time) = 0;


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		Camera&    GetCamera() const { return *camera; }
		const Fog& GetFog()    const { return fog; }

		const vector<DirectionalLight>& GetDirectionalLights() const { return directional_lights; }
		const vector<PointLight>&       GetPointLights() const { return point_lights; }
		const vector<SpotLight>&        GetSpotLights() const { return spot_lights; }


	protected:
		Scene() : enable_input(true) {}
		virtual void Init(ID3D11Device* device,
						  ID3D11DeviceContext* device_context,
						  ResourceMgr& resource_mgr) = 0;


	protected:
		bool                     enable_input;

		unique_ptr<Camera>       camera;

		vector<Model>            models;

		map<string, Text>        texts;

		vector<PointLight>       point_lights;
		vector<DirectionalLight> directional_lights;
		vector<SpotLight>        spot_lights;

		Fog                      fog;
		SkyBox                   skybox;


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


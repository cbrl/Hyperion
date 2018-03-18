#pragma once

#include "util\datatypes\datatypes.h"
#include "input\input.h"
#include "camera\camera.h"
#include "rendering\buffer\buffers.h"
#include "resource\resource_mgr.h"
#include "resource\model\model.h"
#include "resource\text\text.h"


class Scene {
	public:
		Scene(ID3D11Device* device, ID3D11DeviceContext* device_context, ResourceMgr& resource_mgr);
		~Scene();

		void UpdateMetrics(i32 FPS, i32 CPU, i32 mouse_x, i32 mouse_y);
		void Tick(Input& input, float delta_time);

	private:
		void Init(ID3D11Device* device, ID3D11DeviceContext* device_context, ResourceMgr& resource_mgr);


	public:
		unique_ptr<Camera>       camera;
		vector<PointLight>       point_lights;
		vector<DirectionalLight> directional_lights;
		vector<SpotLight>        spot_lights;
		vector<Model>            models;
		map<string, Text>        texts;


	public:
		template<typename ElementT, typename ActionT>
		void ForEach(ActionT act) {
			if constexpr (is_same_v<Model, ElementT>) {
				for (const auto& e : models) {
					act(e);
				}
			}

			if constexpr (is_same_v<Text, ElementT>) {
				for (auto& e : texts) {
					act(e.second);
				}
			}

			if constexpr (is_same_v<PointLight, ElementT>) {
				for (const auto& e : lights) {
					act(e);
				}
			}

			if constexpr (is_same_v<PointLight, ElementT>) {
				for (const auto& e : lights) {
					act(e);
				}
			}
		}
};


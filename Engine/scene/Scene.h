#pragma once

#include <map>
#include <string>
#include <vector>
#include <type_traits>
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "input\input.h"
#include "camera\camera.h"
#include "rendering\buffer\buffers.h"
#include "geometry\model\model.h"
#include "text\text.h"
#include "loader\obj_loader.h"

using std::map;
using std::string;
using std::wstring;
using std::to_wstring;
using std::vector;
using std::is_same_v;

class Scene {
	public:
		Scene();
		~Scene();

		void UpdateMetrics(int FPS, int CPU, int mouseX, int mouseY);
		void Tick(Input& input, float deltaTime);
		void Render(float deltaTime);

	private:
		void Init(ID3D11Device* device, ID3D11DeviceContext* device_context);


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


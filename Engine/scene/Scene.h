#pragma once

#include <map>
#include <string>
#include <vector>
#include <type_traits>
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "input\Input.h"
#include "camera\Camera.h"
#include "light\Light.h"
#include "geometry\model\Model.h"
#include "text\Text.h"
#include "loader\OBJLoader.h"

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
		unique_ptr<Camera>    camera;
		vector<Light>         lights;
		vector<Model>         models;
		map<string, Text>     texts;


	public:
		template<typename Element, typename Action>
		void ForEach(Action act) {
			if constexpr (is_same_v<Model, Element>) {
				for (auto& e : models) {
					act(e);
				}
			}

			if constexpr (is_same_v<Text, Element>) {
				for (auto& e : texts) {
					act(e.second);
				}
			}

			if constexpr (is_same_v<Light, Element>) {
				for (auto& e : lights) {
					act(e);
				}
			}
		}
};


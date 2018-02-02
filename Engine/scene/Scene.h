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
#include "texture\TextureMgr.h"

using std::map;
using std::wstring;
using std::to_wstring;
using std::vector;
using std::is_same_v;

class Scene {
	public:
		Scene(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
		~Scene();

		bool Init();
		void UpdateMetrics(int FPS, int CPU, int mouseX, int mouseY);
		void Tick(Input& input, float deltaTime);
		bool Render(float deltaTime);


	public:
		unique_ptr<Camera>    camera;
		vector<Light>         lights;
		vector<Model>         models;
		map<wstring, Text>    texts;


	private:
	    HWND                        hWnd;
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		unique_ptr<TextureMgr>      textureMgr;

		float rotation = 0.0f;


	public:
		template<typename Element, typename Action>
		void ForEach(Action act) {
			if constexpr (is_same_v<Model, Element>) {
				for (auto &e : models) {
					act(e);
				}
			}

			if constexpr (is_same_v<Text, Element>) {
				for (auto &e : texts) {
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


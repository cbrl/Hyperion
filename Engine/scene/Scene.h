#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include <vector>
#include <type_traits>

#include "input\Input.h"
#include "camera\Camera.h"
#include "light\Light.h"
#include "model\Model.h"
#include "text\Text.h"
#include "texture\TextureMgr.h"

using std::map;
using std::wstring;
using std::to_wstring;
using std::vector;
using std::is_same_v;

class Scene {
	public:
		Scene(HWND hWnd, ComPtr<ID3D11Device> m_Device, ComPtr<ID3D11DeviceContext> m_DeviceContext);
		~Scene();

		bool Init();
		void UpdateMetrics(int FPS, int CPU, int mouseX, int mouseY);
		void Tick(Input& input, float deltaTime);


	public:
		unique_ptr<Camera>    m_Camera;
		vector<Light>         m_Lights;
		vector<Model>         m_Models;
		map<wstring, Text>    m_Texts;


	private:
	    HWND                        m_hWnd;
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		unique_ptr<TextureMgr>      m_TextureMgr;

		XMFLOAT3 m_Rotation;


	public:
		template<typename Element, typename Action>
		void ForEach(Action act) {
			if constexpr (is_same_v<Model, Element>) {
				for (auto &e : m_Models) {
					act(e);
				}
			}

			if constexpr (is_same_v<Text, Element>) {
				for (auto &e : m_Texts) {
					act(e.second);
				}
			}

			if constexpr (is_same_v<Light, Element>) {
				for (auto& e : m_Lights) {
					act(e);
				}
			}
		}
};


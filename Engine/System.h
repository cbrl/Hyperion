#pragma once

#include "Direct3D.h"
#include <d3d11.h>
#include <wrl\client.h>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;

class System {
	public:
		System(HWND hWnd, int windowWidth, int windowHeight);
		~System();

		bool Init();
		bool Tick();
		void OnResize(int windowWidth, int windowHeight);


	private:
		unique_ptr<Direct3D> m_D3DApp;

		HWND      m_hWnd;
		int       m_WindowWidth;
		int       m_WindowHeight;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
};

#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "direct3d\direct3d.h"
#include "rendering\render_state_mgr.h"
#include "rendering\renderer.h"
#include "texture\texture_mgr.h"


class RenderingMgr {
	public:
		static const RenderingMgr* Get();

		RenderingMgr(HWND window);
		~RenderingMgr();

		void Init(UINT window_width, UINT window_height, bool fullscreen, bool vsync, bool msaa);


		//----------------------------------------------------------------------------------
		// Get device / device context
		//----------------------------------------------------------------------------------

		static ID3D11Device* GetDevice() {
			return Direct3D::Get()->GetDevice();
		}
		static ID3D11DeviceContext* GetDeviceContext() {
			return Direct3D::Get()->GetDeviceContext();
		}


		//----------------------------------------------------------------------------------
		// Get members
		//----------------------------------------------------------------------------------

		Direct3D* GetD3D() const {
			return direct3D.get();
		}

		Renderer* GetRenderer() const {
			return renderer.get();
		}

		RenderStateMgr* GetRenderStateMgr() const {
			return render_state_mgr.get();
		}

		TextureMgr* GetTextureMgr() const {
			return texture_mgr.get();
		}


	private:
		unique_ptr<Direct3D>       direct3D;
		unique_ptr<RenderStateMgr> render_state_mgr;
		unique_ptr<TextureMgr>     texture_mgr;
		unique_ptr<Renderer>       renderer;

		HWND hWnd;
};
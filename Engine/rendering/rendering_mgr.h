#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "direct3d\direct3d.h"
#include "rendering\render_state_mgr.h"
#include "rendering\renderer.h"
#include "resource\resource_mgr.h"
#include "scene\scene.h"


class RenderingMgr {
	public:
		RenderingMgr(u32 window_width, u32 window_height, bool fullscreen, bool vsync, bool msaa);
		~RenderingMgr() = default;


		void OnResize(u32 window_width, u32 window_height) const {
			direct3D->OnResize(window_width, window_height);
		}

		void Render(Scene& scene) const;


		//----------------------------------------------------------------------------------
		// Get device / device context
		//----------------------------------------------------------------------------------

		ID3D11Device* GetDevice() {
			return direct3D->GetDevice();
		}
		ID3D11DeviceContext* GetDeviceContext() {
			return direct3D->GetDeviceContext();
		}


		//----------------------------------------------------------------------------------
		// Get members
		//----------------------------------------------------------------------------------

		const Direct3D* GetD3D() const {
			return direct3D.get();
		}

		ResourceMgr* GetResourceMgr() const {
			return resource_mgr.get();
		}


	private:
		unique_ptr<Direct3D>       direct3D;
		unique_ptr<RenderStateMgr> render_state_mgr;
		unique_ptr<Renderer>       renderer;
		unique_ptr<ResourceMgr>    resource_mgr;
};
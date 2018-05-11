#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "rendering\direct3d\direct3d.h"
#include "rendering\render_state_mgr.h"
#include "rendering\renderer\renderer.h"
#include "resource\resource_mgr.h"
#include "imgui\imgui_impl_dx11.h"
#include "scene\scene.h"

class Engine;

class RenderingMgr final {
	public:
		RenderingMgr(Engine& engine, bool fullscreen, bool vsync, bool msaa);
		~RenderingMgr();

		// Resize the viewport
		void ResizeBuffers(u32 window_width, u32 window_height) const {
			direct3D->ResizeBuffers(window_width, window_height);
			ImGui_ImplDX11_InvalidateDeviceObjects();
			ImGui_ImplDX11_CreateDeviceObjects();
		}

		// Render the scene
		void Render(Engine& engine) const;


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		ID3D11Device& GetDevice() const {
			return direct3D->GetDevice();
		}

		ID3D11DeviceContext& GetDeviceContext() const {
			return direct3D->GetDeviceContext();
		}

		ResourceMgr& GetResourceMgr() const {
			return *resource_mgr;
		}

		const RenderStateMgr& GetRenderStateMgr() const {
			return *render_state_mgr;
		}


	private:
		unique_ptr<Direct3D>       direct3D;
		unique_ptr<RenderStateMgr> render_state_mgr;
		unique_ptr<Renderer>       renderer;
		unique_ptr<ResourceMgr>    resource_mgr;
};
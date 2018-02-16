#pragma once

#include <wrl\client.h>
#include <Windows.h>
#include <d3d11.h>

#include "util\EngineUtil.h"
#include "direct3d\Direct3D.h"
#include "shader\ShaderMgr.h"
#include "buffer\CBufferMgr.h"
#include "rendering\RenderStateMgr.h"
#include "rendering\Renderer.h"
#include "texture\TextureMgr.h"

using std::unique_ptr;
using std::make_unique;

class RenderingMgr {
	public:
		static const RenderingMgr* Get();

		RenderingMgr(HWND window);
		~RenderingMgr();

		void Init(UINT window_width, UINT window_height, bool fullscreen, bool vsync, bool msaa);
		
		void BindShader(ShaderTypes shader) const {
			shader_mgr->BindShader(direct3D->GetDeviceContext(), shader);
		}

		template<typename DataT>
		void UpdateData(const DataT& data) const {
			cbuffer_mgr->UpdateData(direct3D->GetDeviceContext(), data);
		}


		Direct3D* GetD3D() const {
			return direct3D.get();
		}

		Renderer* GetRenderer() const {
			return renderer.get();
		}

		RenderStateMgr* GetRenderStateMgr() const {
			return render_state_mgr.get();
		}

		CBufferMgr* GetCBufferMgr() const {
			return cbuffer_mgr.get();
		}

		TextureMgr* GetTextureMgr() const {
			return texture_mgr.get();
		}


	private:
		unique_ptr<Direct3D>       direct3D;
		unique_ptr<RenderStateMgr> render_state_mgr;
		unique_ptr<ShaderMgr>      shader_mgr;
		unique_ptr<CBufferMgr>     cbuffer_mgr;
		unique_ptr<TextureMgr>     texture_mgr;
		unique_ptr<Renderer>       renderer;

		HWND hWnd;
};
#pragma once

#include <wrl\client.h>
#include <Windows.h>
#include <d3d11.h>

#include "util\EngineUtil.h"
#include "direct3d\direct3d.h"
#include "shader\ShaderMgr.h"
#include "buffer\CBufferMgr.h"
#include "rendering\RenderStateMgr.h"
#include "rendering\Renderer.h"

using std::unique_ptr;
using std::make_unique;

class RenderingMgr {
	public:
		static const RenderingMgr* Get();

		RenderingMgr(HWND window);
		~RenderingMgr();

		void Init(UINT windowWidth, UINT windowHeight, bool fullscreen, bool vsync, bool msaa);
		
		void BindShader(ShaderTypes shader) const {
			shaderMgr->BindShader(shader);
		}

		template<typename DataT>
		void UpdateData(const DataT& data) const {
			cBufferMgr->UpdateData(data);
		}


		Direct3D* GetD3D() const {
			return direct3D.get();
		}

		Renderer* GetRenderer() const {
			return renderer.get();
		}

		RenderStateMgr* GetRenderStateMgr() const {
			return renderStateMgr.get();
		}

		CBufferMgr* GetCBufferMgr() const {
			return cBufferMgr.get();
		}


	private:
		unique_ptr<Direct3D>       direct3D;
		unique_ptr<RenderStateMgr> renderStateMgr;
		unique_ptr<ShaderMgr>      shaderMgr;
		unique_ptr<CBufferMgr>     cBufferMgr;
		unique_ptr<Renderer>       renderer;

		HWND hWnd;
};
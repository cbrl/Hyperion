#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl\client.h>

#include "direct3d\direct3d.h"
#include "rendering\RenderStateMgr.h"
#include "rendering\Renderer.h"
#include "buffer\CBufferMgr.h"

using std::unique_ptr;
using std::make_unique;

class RenderingMgr {
	public:
		static const RenderingMgr* Get();

		RenderingMgr(HWND window);
		~RenderingMgr();

		bool Init(UINT windowWidth, UINT windowHeight, bool fullscreen, bool vsync, bool msaa);
		
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
		unique_ptr<CBufferMgr>     cBufferMgr;
		unique_ptr<Renderer>       renderer;

		HWND hWnd;
};
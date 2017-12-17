#pragma once

class Direct3D {
	public:
		Direct3D(HWND hWnd, int windowWidth, int windowHeight, bool MSAA, bool vSync, bool fullscreen);
		~Direct3D();

		bool Init();
		void OnResize();


	private:
		void GetRefreshRate();


	private:
		ID3D11Device           *m_Device;
		ID3D11DeviceContext    *m_DeviceContext;
		IDXGISwapChain         *m_SwapChain;
		ID3D11Texture2D        *m_DepthStencilBuffer;
		ID3D11DepthStencilView *m_DepthStencilView;
		ID3D11RenderTargetView *m_RenderTargetView;
		D3D11_VIEWPORT          m_WindowViewport;
		D3D_DRIVER_TYPE         m_DriverType;

		HWND m_hWnd;
		int  m_WindowWidth;
		int  m_WindowHeight;
		bool m_EnableVSync;
		bool m_EnableFullscreen;
		UINT m_Numerator;
		UINT m_Denominator;

		UINT m_4xMSAAQuality;
		bool m_Enable4xMSAA;
};


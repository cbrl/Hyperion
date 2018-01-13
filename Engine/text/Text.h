#pragma once

#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXColors.h>
#include <string>
#include <memory>
#include <Windows.h>
#include <wrl\client.h>

using std::wstring;
using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Text {
	public:
		Text(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, const WCHAR* fontFilename);
		~Text();

		void SetText(wstring text);
		void SetText(wstring text, XMFLOAT2 position);
		void SetText(wstring text, XMFLOAT2 position, XMVECTORF32 color);
		void SetText(wstring text, XMFLOAT2 position, XMVECTORF32 color, float rotation);

		void SetPosition(XMFLOAT2 position);
		void SetColor(XMVECTORF32 color);
		void SetRotation(float rotation);

		void Render();


	private:
	unique_ptr<SpriteFont>  m_SpriteFont;
	unique_ptr<SpriteBatch> m_SpriteBatch;

	wstring     m_Text;
	XMFLOAT2    m_Position;
	XMVECTORF32 m_Color;
	float       m_Rotation;
};


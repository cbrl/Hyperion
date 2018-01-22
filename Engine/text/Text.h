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
		Text(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* fontFilename);
		~Text();

		void SetText(wstring newText);
		void SetText(wstring newText, XMFLOAT2 newPosition);
		void SetText(wstring newText, XMFLOAT2 newPosition, XMVECTORF32 newColor);
		void SetText(wstring newText, XMFLOAT2 newPosition, XMVECTORF32 newColor, float newRotation);

		void SetPosition(XMFLOAT2 newPosition);
		void SetColor(XMVECTORF32 newColor);
		void SetRotation(float newRotation);

		void Render();


	private:
		unique_ptr<SpriteFont>  m_SpriteFont;
		unique_ptr<SpriteBatch> m_SpriteBatch;

		wstring     text;
		XMFLOAT2    position;
		XMVECTORF32 color;
		float       rotation;
};


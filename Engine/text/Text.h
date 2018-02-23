#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXColors.h>
#include "util\datatypes\datatypes.h"


using namespace DirectX;


class Text {
	public:
		Text(ID3D11Device* device, ID3D11DeviceContext* device_context, const WCHAR* fontFilename);
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
		unique_ptr<SpriteFont>  sprite_font;
		unique_ptr<SpriteBatch> sprite_batch;

		wstring     text;
		XMFLOAT2    position;
		XMVECTORF32 color;
		float       rotation;
};


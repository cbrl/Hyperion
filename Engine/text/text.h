#pragma once

#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXColors.h>
#include "util\datatypes\datatypes.h"
#include "resource\resource_mgr.h"


class Text {
	public:
		Text(ID3D11DeviceContext* device_context, ResourceMgr& resource_mgr, const wstring& font_filename);
		~Text();

		void SetText(wstring new_text);
		void SetText(wstring new_text, float2 new_position);
		void SetText(wstring new_text, float2 new_position, XMVECTORF32 new_color);
		void SetText(wstring new_text, float2 new_position, XMVECTORF32 new_color, float new_rotation);

		void SetPosition(float2 new_position);
		void SetColor(XMVECTORF32 new_color);
		void SetRotation(float new_rotation);

		void Render();


	private:
		shared_ptr<SpriteFont>  sprite_font;
		unique_ptr<SpriteBatch> sprite_batch;

		wstring     text;
		float2      position;
		XMVECTORF32 color;
		float       rotation;
};


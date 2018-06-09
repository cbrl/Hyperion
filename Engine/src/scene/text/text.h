#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include "datatypes/datatypes.h"
#include "resource/resource_mgr.h"


class Text final {
public:
	Text(ResourceMgr& resource_mgr, const wstring& font_filename);
	~Text() = default;

	const SpriteFont& GetFont() const;
	const wstring& GetText() const;
	const float2& GetPosition() const;
	const XMVECTORF32& GetColor() const;
	float GetRotation() const;

	void SetText(wstring new_text);
	void SetText(wstring new_text, float2 new_position);
	void SetText(wstring new_text, float2 new_position, XMVECTORF32 new_color);
	void SetText(wstring new_text, float2 new_position, XMVECTORF32 new_color, float new_rotation);

	void SetPosition(float2 new_position);
	void SetColor(XMVECTORF32 new_color);
	void SetRotation(float new_rotation);


private:
	shared_ptr<Font> font;

	wstring text;
	float2 position;
	XMVECTORF32 color;
	float rotation;
};

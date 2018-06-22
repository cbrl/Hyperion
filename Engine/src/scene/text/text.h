#pragma once

#include "directx/d3d11.h"
#include <DirectXColors.h>
#include "datatypes/datatypes.h"
#include "resource/resource_mgr.h"


class Text final {
public:
	Text(ResourceMgr& resource_mgr, const wstring& font_filename);
	~Text() = default;


	[[nodiscard]]
	const SpriteFont& getFont() const;

	[[nodiscard]]
	const wstring& getText() const;

	[[nodiscard]]
	const float2& getPosition() const;

	[[nodiscard]]
	const XMVECTORF32& getColor() const;

	[[nodiscard]]
	float getRotation() const;


	void setText(wstring new_text);
	void setText(wstring new_text, float2 new_position);
	void setText(wstring new_text, float2 new_position, XMVECTORF32 new_color);
	void setText(wstring new_text, float2 new_position, XMVECTORF32 new_color, float new_rotation);

	void setPosition(float2 new_position);
	void setColor(XMVECTORF32 new_color);
	void setRotation(float new_rotation);


private:
	shared_ptr<Font> font;

	wstring text;
	float2 position;
	XMVECTORF32 color;
	float rotation;
};

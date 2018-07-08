#pragma once

#include <DirectXColors.h>
#include "directx/directxtk.h"
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
	XMVECTOR XM_CALLCONV getPosition() const;

	[[nodiscard]]
	const XMVECTORF32& getColor() const;

	[[nodiscard]]
	f32 getRotation() const;


	void setText(wstring new_text);
	void setText(wstring new_text, vec2_f32 new_position);
	void setText(wstring new_text, vec2_f32 new_position, XMVECTORF32 new_color);
	void setText(wstring new_text, vec2_f32 new_position, XMVECTORF32 new_color, f32 new_rotation);

	void setPosition(vec2_f32 new_position);
	void setColor(XMVECTORF32 new_color);
	void setRotation(f32 new_rotation);


private:
	shared_ptr<Font> font;

	wstring text;
	XMVECTOR position;
	XMVECTORF32 color;
	f32 rotation;
};

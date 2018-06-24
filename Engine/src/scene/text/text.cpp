#include "stdafx.h"
#include "text.h"


Text::Text(ResourceMgr& resource_mgr, const wstring& font_filename)
	: text(L"Default Text")
	, position(XMVectorZero())
	, color(Colors::White)
	, rotation(0.0f) {

	font = resource_mgr.getOrCreate<Font>(font_filename);
}

void Text::setText(wstring new_text) {
	text = new_text;
}


void Text::setText(wstring new_text, f32_2 new_position) {
	text     = new_text;
	position = XMLoad(&new_position);
}


void Text::setText(wstring new_text, f32_2 new_position, XMVECTORF32 new_color) {
	text     = new_text;
	position = XMLoad(&new_position);
	color    = new_color;
}


void Text::setText(wstring new_text, f32_2 new_position, XMVECTORF32 new_color, f32 new_rotation) {
	text     = new_text;
	position = XMLoad(&new_position);
	color    = new_color;
	rotation = new_rotation;
}


void Text::setPosition(f32_2 new_position) {
	position = XMLoad(&new_position);
}


void Text::setColor(XMVECTORF32 new_color) {
	color = new_color;
}


void Text::setRotation(f32 new_rotation) {
	rotation = new_rotation;
}


[[nodiscard]]
const SpriteFont& Text::getFont() const {
	return *font.get();
}


[[nodiscard]]
const wstring& Text::getText() const {
	return text;
}


[[nodiscard]]
XMVECTOR XM_CALLCONV Text::getPosition() const {
	return position;
}


[[nodiscard]]
const XMVECTORF32& Text::getColor() const {
	return color;
}


[[nodiscard]]
f32 Text::getRotation() const {
	return rotation;
}

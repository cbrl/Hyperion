#include "stdafx.h"
#include "text.h"


Text::Text(ResourceMgr& resource_mgr, const wstring& font_filename)
	: text(L"Default Text")
	, position(float2(0, 0))
	, color(Colors::White)
	, rotation(0.0f) {

	font = resource_mgr.getOrCreate<Font>(font_filename);
}

void Text::setText(wstring new_text) {
	text = new_text;
}


void Text::setText(wstring new_text, float2 new_position) {
	text     = new_text;
	position = new_position;
}


void Text::setText(wstring new_text, float2 new_position, XMVECTORF32 new_color) {
	text     = new_text;
	position = new_position;
	color    = new_color;
}


void Text::setText(wstring new_text, float2 new_position, XMVECTORF32 new_color, float new_rotation) {
	text     = new_text;
	position = new_position;
	color    = new_color;
	rotation = new_rotation;
}


void Text::setPosition(float2 new_position) {
	position = new_position;
}


void Text::setColor(XMVECTORF32 new_color) {
	color = new_color;
}


void Text::setRotation(float new_rotation) {
	rotation = new_rotation;
}


const SpriteFont& Text::getFont() const {
	return *font.get();
}


const wstring& Text::getText() const {
	return text;
}


const float2& Text::getPosition() const {
	return position;
}


const XMVECTORF32& Text::getColor() const {
	return color;
}


float Text::getRotation() const {
	return rotation;
}

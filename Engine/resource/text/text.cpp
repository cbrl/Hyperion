#include "stdafx.h"
#include "text.h"


Text::Text(ID3D11Device* device, ID3D11DeviceContext* device_context, const WCHAR* fontFilename)
	: text(L"Default Text")
	, position(float2(0, 0))
	, color(Colors::White)
	, rotation(0.0f)
{
	sprite_batch = make_unique<SpriteBatch>(device_context);
	sprite_font = make_unique<SpriteFont>(device, fontFilename);
}


Text::~Text() {
}


void Text::SetText(wstring new_text) {
	text = new_text;
}


void Text::SetText(wstring new_text, float2 new_position) {
	text     = new_text;
	position = position;
}


void Text::SetText(wstring new_text, float2 new_position, XMVECTORF32 new_color) {
	text     = new_text;
	position = new_position;
	color    = new_color;
}


void Text::SetText(wstring new_text, float2 new_position, XMVECTORF32 new_color, float new_rotation) {
	text     = new_text;
	position = new_position;
	color    = new_color;
	rotation = new_rotation;
}


void Text::SetPosition(float2 new_position) {
	position = new_position;
}


void Text::SetColor(XMVECTORF32 new_color) {
	color = new_color;
}


void Text::SetRotation(float new_rotation) {
	rotation = new_rotation;
}


void Text::Render() {
	sprite_batch->Begin();
	sprite_font->DrawString(sprite_batch.get(), &text[0], position, color, rotation);
	sprite_batch->End();
}

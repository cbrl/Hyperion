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


void Text::SetText(wstring newText) {
	text = newText;
}


void Text::SetText(wstring newText, float2 newPosition) {
	text     = newText;
	position = position;
}


void Text::SetText(wstring newText, float2 newPosition, XMVECTORF32 newColor) {
	text     = newText;
	position = newPosition;
	color    = newColor;
}


void Text::SetText(wstring newText, float2 newPosition, XMVECTORF32 newColor, float newRotation) {
	text     = newText;
	position = newPosition;
	color    = newColor;
	rotation = newRotation;
}


void Text::SetPosition(float2 newPosition) {
	position = newPosition;
}


void Text::SetColor(XMVECTORF32 newColor) {
	color = newColor;
}


void Text::SetRotation(float newRotation) {
	rotation = newRotation;
}


void Text::Render() {
	sprite_batch->Begin();
	sprite_font->DrawString(sprite_batch.get(), &text[0], position, color, rotation);
	sprite_batch->End();
}

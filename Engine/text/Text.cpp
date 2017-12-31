#include "stdafx.h"
#include "Text.h"


Text::Text(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, const WCHAR* fontFilename) :
	m_Text(L"Default Text"),
	m_Position(XMFLOAT2(0, 0)),
	m_Color(Colors::White),
	m_Rotation(0.0f)
{
	m_SpriteBatch = make_unique<SpriteBatch>(deviceContext.Get());
	m_SpriteFont = make_unique<SpriteFont>(device.Get(), fontFilename);
}


Text::~Text() {
}


void Text::SetText(wstring text) {
	m_Text = text;
}


void Text::SetText(wstring text, XMFLOAT2 position) {
	m_Text     = text;
	m_Position = position;
}


void Text::SetText(wstring text, XMFLOAT2 position, XMVECTORF32 color) {
	m_Text     = text;
	m_Position = position;
	m_Color    = color;
}


void Text::SetText(wstring text, XMFLOAT2 position, XMVECTORF32 color, float rotation) {
	m_Text     = text;
	m_Position = position;
	m_Color    = color;
	m_Rotation = rotation;
}


void Text::SetPosition(XMFLOAT2 position) {
	m_Position = position;
}


void Text::SetColor(XMVECTORF32 color) {
	m_Color = color;
}


void Text::SetRotation(float rotation) {
	m_Rotation = rotation;
}


void Text::Render() {
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), &m_Text[0], m_Position, m_Color, m_Rotation);
	m_SpriteBatch->End();
}

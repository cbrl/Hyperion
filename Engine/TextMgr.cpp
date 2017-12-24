#include "stdafx.h"
#include "TextMgr.h"


TextMgr::TextMgr(const ComPtr<ID3D11Device> device, const ComPtr<ID3D11DeviceContext> deviceContext) :
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}


TextMgr::~TextMgr() {
	m_TextMap.clear();
}


void TextMgr::CreateTextObject(wstring label, const WCHAR* fontFilename) {

	// Create the text object only if it doesn't already exist
	if (m_TextMap.find(label) == m_TextMap.end()) {
		m_TextMap[label] = make_shared<Text>(m_Device, m_DeviceContext, fontFilename);
	}
}


shared_ptr<Text> TextMgr::GetTextObject(wstring label) {
	// Check if the object requested exists
	if (m_TextMap.find(label) != m_TextMap.end()) {
		return m_TextMap[label];
	}

	// Return nullptr if it doesn't exist
	return nullptr;
}

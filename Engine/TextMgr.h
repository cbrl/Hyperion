#pragma once

#include "Text.h"
#include <d3d11.h>
#include <map>
#include <string>
#include <optional>

using std::map;
using std::wstring;
using std::shared_ptr;
using std::make_shared;
using std::optional;

class TextMgr {
	public:
		TextMgr(const ComPtr<ID3D11Device> device, const ComPtr<ID3D11DeviceContext> deviceContext);
		~TextMgr();

		void CreateTextObject(wstring textLabel, const WCHAR* fontFilename);

		shared_ptr<Text> GetTextObject(wstring textLabel);


	private:
		map<wstring, shared_ptr<Text>> m_TextMap;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
};


#pragma once

#include <SpriteFont.h>
#include "resource\resource.h"


// Simple wrapper around SpriteFont and Resource base class
class Font : public SpriteFont, public Resource<Font> {
	public:
		Font(_In_ ID3D11Device* device, _In_z_ wchar_t const* fileName, bool forceSRGB = false)
			: SpriteFont(device, fileName, forceSRGB) 
			, Resource(fileName) {
		}

		Font(_In_ ID3D11Device* device, _In_reads_bytes_(dataSize) uint8_t const* dataBlob, _In_ size_t dataSize, bool forceSRGB = false)
			: SpriteFont(device, dataBlob, dataSize, forceSRGB) {
		}

		Font(_In_ ID3D11ShaderResourceView* texture, _In_reads_(glyphCount) Glyph const* glyphs, _In_ size_t glyphCount, _In_ float lineSpacing)
			: SpriteFont(texture, glyphs, glyphCount, lineSpacing) {
		}

		Font(SpriteFont&& moveFrom)
			: SpriteFont(std::move(moveFrom)) {
		}

		Font& operator= (SpriteFont&& moveFrom) {
			SpriteFont::operator=(std::move(moveFrom));
		}

		Font(SpriteFont const&) = delete;
		Font(Font const&) = delete;
		Font& operator= (SpriteFont const&) = delete;
		Font& operator= (Font const&) = delete;
};
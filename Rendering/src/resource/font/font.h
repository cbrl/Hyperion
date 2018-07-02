#pragma once

#include <SpriteFont.h>
#include "resource/resource.h"


// Simple wrapper around SpriteFont and Resource base class
class Font final : public SpriteFont, public Resource<Font> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Font(_In_ ID3D11Device& device, const wchar_t* file, bool forceSRGB = false)
		: SpriteFont(&device, file, forceSRGB)
		, Resource(file) {
	}

	Font(const Font& font) = delete;
	Font(Font&& font) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Font() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Font& operator=(const Font& font) = delete;
	Font& operator=(Font&& font) noexcept = default;
};

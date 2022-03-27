module;

#include "directx/directxtk.h"
#include "string/string.h"

#include "directx/d3d11.h"

export module rendering.font;

export import rendering.resource;


namespace render {

// Simple wrapper around SpriteFont and Resource base class
export class Font final : public Resource<Font> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Font(ID3D11Device& device, gsl::cwzstring<> file, bool forceSRGB = false)
		: Resource(file)
		, font(&device, file, forceSRGB) {
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


	//----------------------------------------------------------------------------------
	// Member Function - Font Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	SpriteFont& getSpriteFont() noexcept {
		return font;
	}

	[[nodiscard]]
	const SpriteFont& getSpriteFont() const noexcept {
		return font;
	}

private:
	SpriteFont font;
};

} //namespace render

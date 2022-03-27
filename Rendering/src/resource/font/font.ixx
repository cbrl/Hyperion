module;

#include <SpriteFont.h>
#include "datatypes/types.h"

export module rendering.font;

export import rendering.resource;


namespace render {

// Simple wrapper around SpriteFont and Resource base class
export class Font final : public DirectX::SpriteFont, public Resource<Font> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Font(ID3D11Device& device, gsl::cwzstring<> file, bool forceSRGB = false)
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

} //namespace render

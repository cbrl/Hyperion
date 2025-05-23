module;

#include <memory>

#include <DirectXMath.h>

#include "directx/directxtk.h"

export module rendering:components.text;

import ecs;
import :font;

using namespace DirectX;

export class Text final : public ecs::Component {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Text(std::shared_ptr<render::Font> font)
		: font(std::move(font))
		, text(L"Default Text")
		, color(Colors::White) {
	}

	Text(const Text& text) = delete;
	Text(Text&& text) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Text() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Text& operator=(const Text& text) = delete;
	Text& operator=(Text&& text) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Font
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const SpriteFont& getFont() const {
		return font->getSpriteFont();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Text
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::wstring& getText() const {
		return text;
	}

	void setText(const std::wstring& new_text) {
		text = new_text;
	}

	void setText(const std::wstring& new_text, XMVECTORF32 new_color) {
		text  = new_text;
		color = new_color;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Color
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTORF32 XM_CALLCONV getColor() const {
		return color;
	}

	void setColor(XMVECTORF32 new_color) {
		color = new_color;
	}



private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::shared_ptr<render::Font> font;

	std::wstring text;
	XMVECTORF32  color;
};
#pragma once

#include "directx/d3d11.h"
#include "datatypes/vector_types.h"


struct VertexPosition final {
	constexpr VertexPosition() noexcept = default;
	constexpr VertexPosition(const VertexPosition& vertex) noexcept = default;
	constexpr VertexPosition(VertexPosition&& vertex) noexcept = default;

	constexpr VertexPosition(const f32_3& position) noexcept
		: position(position) {
	}

	~VertexPosition() = default;

	constexpr VertexPosition& operator=(const VertexPosition& vertex) noexcept = default;
	constexpr VertexPosition& operator=(VertexPosition&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;

	static const u32 input_element_count = 1;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};


struct VertexPositionColor final {
	constexpr VertexPositionColor() noexcept = default;
	constexpr VertexPositionColor(const VertexPositionColor& vertex) noexcept = default;
	constexpr VertexPositionColor(VertexPositionColor&& vertex) noexcept = default;

	constexpr VertexPositionColor(const f32_3& position, const f32_4& color) noexcept
		: position(position)
		, color(color) {
	}

	~VertexPositionColor() = default;

	constexpr VertexPositionColor& operator=(const VertexPositionColor& vertex) noexcept = default;
	constexpr VertexPositionColor& operator=(VertexPositionColor&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return true; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_4 color;

	static const u32 input_element_count = 2;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};


struct VertexPositionTexture final {
	constexpr VertexPositionTexture() noexcept = default;
	constexpr VertexPositionTexture(const VertexPositionTexture& vertex) noexcept = default;
	constexpr VertexPositionTexture(VertexPositionTexture&& vertex) noexcept = default;

	constexpr VertexPositionTexture(const f32_3& position, const f32_2& texCoord) noexcept
		: position(position)
		, texCoord(texCoord) {
	}

	~VertexPositionTexture() = default;

	constexpr VertexPositionTexture& operator=(const VertexPositionTexture& vertex) noexcept = default;
	constexpr VertexPositionTexture& operator=(VertexPositionTexture&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_2 texCoord;

	static const u32 input_element_count = 2;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};


struct VertexPositionDualTexture final {
	constexpr VertexPositionDualTexture() noexcept = default;
	constexpr VertexPositionDualTexture(const VertexPositionDualTexture& vertex) noexcept = default;
	constexpr VertexPositionDualTexture(VertexPositionDualTexture&& vertex) noexcept = default;

	constexpr VertexPositionDualTexture(const f32_3& position,
	                                    const f32_2& texCoord0,
	                                    const f32_2& texCoord1) noexcept
		: position(position)
		, texCoord0(texCoord0)
		, texCoord1(texCoord1) {
	}

	~VertexPositionDualTexture() = default;

	constexpr VertexPositionDualTexture& operator=(const VertexPositionDualTexture& vertex) noexcept = default;
	constexpr VertexPositionDualTexture& operator=(VertexPositionDualTexture&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_2 texCoord0;
	f32_2 texCoord1;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[InputElementCount];
};


struct VertexPositionNormal final {
	constexpr VertexPositionNormal() noexcept = default;
	constexpr VertexPositionNormal(const VertexPositionNormal& vertex) noexcept = default;
	constexpr VertexPositionNormal(VertexPositionNormal&& vertex) noexcept = default;

	constexpr VertexPositionNormal(const f32_3& position, const f32_3& normal) noexcept
		: position(position)
		, normal(normal) {
	}

	~VertexPositionNormal() = default;

	constexpr VertexPositionNormal& operator=(const VertexPositionNormal& vertex) noexcept = default;
	constexpr VertexPositionNormal& operator=(VertexPositionNormal&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_3 normal;

	static const u32 input_element_count = 2;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};


struct VertexPositionNormalColor final {
	constexpr VertexPositionNormalColor() noexcept = default;
	constexpr VertexPositionNormalColor(const VertexPositionNormalColor& vertex) noexcept = default;
	constexpr VertexPositionNormalColor(VertexPositionNormalColor&& vertex) noexcept = default;

	constexpr VertexPositionNormalColor(const f32_3& position,
	                                    const f32_3& normal,
	                                    const f32_4& color) noexcept
		: position(position)
		, normal(normal)
		, color(color) {
	}

	~VertexPositionNormalColor() = default;

	constexpr VertexPositionNormalColor& operator=(const VertexPositionNormalColor& vertex) noexcept = default;
	constexpr VertexPositionNormalColor& operator=(VertexPositionNormalColor&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return true; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_3 normal;
	f32_4 color;

	static const u32 input_element_count = 3;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};


struct VertexPositionNormalTexture final {
	constexpr VertexPositionNormalTexture() noexcept = default;
	constexpr VertexPositionNormalTexture(const VertexPositionNormalTexture& vertex) noexcept = default;
	constexpr VertexPositionNormalTexture(VertexPositionNormalTexture&& vertex) noexcept = default;

	constexpr VertexPositionNormalTexture(const f32_3& position,
	                                      const f32_3& normal,
	                                      const f32_2& texCoord) noexcept
		: position(position)
		, normal(normal)
		, texCoord(texCoord) {
	}

	~VertexPositionNormalTexture() = default;

	constexpr VertexPositionNormalTexture& operator=(const VertexPositionNormalTexture& vertex) noexcept = default;
	constexpr VertexPositionNormalTexture& operator=(VertexPositionNormalTexture&& vertex) noexcept = default;

	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_3 normal;
	f32_2 texCoord;

	static const u32 input_element_count = 3;
	static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};

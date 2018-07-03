#pragma once

#include "directx/d3d11.h"
#include "datatypes/vector_types.h"


struct VertexPosition final {
	constexpr VertexPosition() noexcept = default;
	constexpr VertexPosition(const VertexPosition& vertex) noexcept = default;
	constexpr VertexPosition(VertexPosition&& vertex) noexcept = default;

	constexpr VertexPosition(vec3_f32 position) noexcept
		: position(position) {
	}

	~VertexPosition() = default;

	constexpr VertexPosition& operator=(const VertexPosition& vertex) noexcept = default;
	constexpr VertexPosition& operator=(VertexPosition&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPosition& compare) noexcept {
		return (this->position == compare.position);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPosition& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;

	static const u32 InputElementCount = 1;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionColor final {
	constexpr VertexPositionColor() noexcept = default;
	constexpr VertexPositionColor(const VertexPositionColor& vertex) noexcept = default;
	constexpr VertexPositionColor(VertexPositionColor&& vertex) noexcept = default;

	constexpr VertexPositionColor(vec3_f32 position, vec4_f32 color) noexcept
		: position(position)
		, color(color) {
	}

	~VertexPositionColor() = default;

	constexpr VertexPositionColor& operator=(const VertexPositionColor& vertex) noexcept = default;
	constexpr VertexPositionColor& operator=(VertexPositionColor&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPositionColor& compare) noexcept {
		return (this->position == compare.position &&
		        this->color    == compare.color);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPositionColor& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return true; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;
	vec4_f32 color;

	static const u32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionTexture final {
	constexpr VertexPositionTexture() noexcept = default;
	constexpr VertexPositionTexture(const VertexPositionTexture& vertex) noexcept = default;
	constexpr VertexPositionTexture(VertexPositionTexture&& vertex) noexcept = default;

	constexpr VertexPositionTexture(vec3_f32 position, vec2_f32 texCoord) noexcept
		: position(position)
		, texCoord(texCoord) {
	}

	~VertexPositionTexture() = default;

	constexpr VertexPositionTexture& operator=(const VertexPositionTexture& vertex) noexcept = default;
	constexpr VertexPositionTexture& operator=(VertexPositionTexture&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPositionTexture& compare) noexcept {
		return (this->position == compare.position &&
		        this->texCoord == compare.texCoord);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPositionTexture& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;
	vec2_f32 texCoord;

	static const u32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionDualTexture final {
	constexpr VertexPositionDualTexture() noexcept = default;
	constexpr VertexPositionDualTexture(const VertexPositionDualTexture& vertex) noexcept = default;
	constexpr VertexPositionDualTexture(VertexPositionDualTexture&& vertex) noexcept = default;

	constexpr VertexPositionDualTexture(vec3_f32 position, vec2_f32 texCoord0, vec2_f32 texCoord1) noexcept
		: position(position)
		, texCoord0(texCoord0)
		, texCoord1(texCoord1) {
	}

	~VertexPositionDualTexture() = default;

	constexpr VertexPositionDualTexture& operator=(const VertexPositionDualTexture& vertex) noexcept = default;
	constexpr VertexPositionDualTexture& operator=(VertexPositionDualTexture&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPositionDualTexture& compare) noexcept {
		return (this->position  == compare.position &&
		        this->texCoord0 == compare.texCoord0 &&
		        this->texCoord1 == compare.texCoord1);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPositionDualTexture& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;
	vec2_f32 texCoord0;
	vec2_f32 texCoord1;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormal final {
	constexpr VertexPositionNormal() noexcept = default;
	constexpr VertexPositionNormal(const VertexPositionNormal& vertex) noexcept = default;
	constexpr VertexPositionNormal(VertexPositionNormal&& vertex) noexcept = default;

	constexpr VertexPositionNormal(vec3_f32 position, vec3_f32 normal) noexcept
		: position(position)
		, normal(normal) {
	}

	~VertexPositionNormal() = default;

	constexpr VertexPositionNormal& operator=(const VertexPositionNormal& vertex) noexcept = default;
	constexpr VertexPositionNormal& operator=(VertexPositionNormal&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPositionNormal& compare) noexcept {
		return (this->position == compare.position &&
		        this->normal   == compare.normal);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPositionNormal& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;
	vec3_f32 normal;

	static const u32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalColor final {
	constexpr VertexPositionNormalColor() noexcept = default;
	constexpr VertexPositionNormalColor(const VertexPositionNormalColor& vertex) noexcept = default;
	constexpr VertexPositionNormalColor(VertexPositionNormalColor&& vertex) noexcept = default;

	constexpr VertexPositionNormalColor(vec3_f32 position, vec3_f32 normal, vec4_f32 color) noexcept
		: position(position)
		, normal(normal)
		, color(color) {
	}

	~VertexPositionNormalColor() = default;

	constexpr VertexPositionNormalColor& operator=(const VertexPositionNormalColor& vertex) noexcept = default;
	constexpr VertexPositionNormalColor& operator=(VertexPositionNormalColor&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPositionNormalColor& compare) noexcept {
		return (this->position == compare.position &&
		        this->normal   == compare.normal &&
		        this->color    == compare.color);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPositionNormalColor& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return true; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;
	vec3_f32 normal;
	vec4_f32 color;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalTexture final {
	constexpr VertexPositionNormalTexture() noexcept = default;
	constexpr VertexPositionNormalTexture(const VertexPositionNormalTexture& vertex) noexcept = default;
	constexpr VertexPositionNormalTexture(VertexPositionNormalTexture&& vertex) noexcept = default;

	constexpr VertexPositionNormalTexture(vec3_f32 position, vec3_f32 normal, vec2_f32 texCoord) noexcept
		: position(position)
		, normal(normal)
		, texCoord(texCoord) {
	}

	~VertexPositionNormalTexture() = default;

	constexpr VertexPositionNormalTexture& operator=(const VertexPositionNormalTexture& vertex) noexcept = default;
	constexpr VertexPositionNormalTexture& operator=(VertexPositionNormalTexture&& vertex) noexcept = default;

	[[nodiscard]]
	constexpr bool operator==(const VertexPositionNormalTexture& compare) noexcept {
		return (this->position == compare.position &&
		        this->normal   == compare.normal &&
		        this->texCoord == compare.texCoord);
	}

	[[nodiscard]]
	constexpr bool operator!=(const VertexPositionNormalTexture& compare) noexcept {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	vec3_f32 position;
	vec3_f32 normal;
	vec2_f32 texCoord;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

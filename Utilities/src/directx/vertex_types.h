#pragma once

#include "datatypes/datatypes.h"

#include <d3d11.h>
using namespace DirectX;


struct VertexPosition {
	VertexPosition() = default;

	VertexPosition(const float3& position)
		: position(position) {
	}

	bool operator==(const VertexPosition& compare) {
		return (this->position == compare.position);
	}

	bool operator!=(const VertexPosition& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return false; }
	static constexpr bool hasColor() { return false; }
	static constexpr bool hasTexture() { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;

	static const u32 InputElementCount = 1;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionColor {
	VertexPositionColor() = default;

	VertexPositionColor(const float3& position, const float4& color)
		: position(position)
		, color(color) {
	}

	bool operator==(const VertexPositionColor& compare) {
		return (this->position == compare.position &&
		        this->color == compare.color);
	}

	bool operator!=(const VertexPositionColor& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return false; }
	static constexpr bool hasColor() { return true; }
	static constexpr bool hasTexture() { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;
	float4 color;

	static const u32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionTexture {
	VertexPositionTexture() = default;

	VertexPositionTexture(const float3& position, const float2& texCoord)
		: position(position)
		, texCoord(texCoord) {
	}

	bool operator==(const VertexPositionTexture& compare) {
		return (this->position == compare.position &&
		        this->texCoord == compare.texCoord);
	}

	bool operator!=(const VertexPositionTexture& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return false; }
	static constexpr bool hasColor() { return false; }
	static constexpr bool hasTexture() { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;
	float2 texCoord;

	static const u32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionDualTexture {
	VertexPositionDualTexture() = default;

	VertexPositionDualTexture(const float3& position, const float2& texCoord0, const float2& texCoord1)
		: position(position)
		, texCoord0(texCoord0)
		, texCoord1(texCoord1) {
	}

	bool operator==(const VertexPositionDualTexture& compare) {
		return (this->position == compare.position &&
		        this->texCoord0 == compare.texCoord0 &&
		        this->texCoord1 == compare.texCoord1);
	}

	bool operator!=(const VertexPositionDualTexture& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return false; }
	static constexpr bool hasColor() { return false; }
	static constexpr bool hasTexture() { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;
	float2 texCoord0;
	float2 texCoord1;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormal {
	VertexPositionNormal() = default;

	VertexPositionNormal(const float3& position, const float3& normal)
		: position(position)
		, normal(normal) {
	}

	bool operator==(const VertexPositionNormal& compare) {
		return (this->position == compare.position &&
		        this->normal == compare.normal);
	}

	bool operator!=(const VertexPositionNormal& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return true; }
	static constexpr bool hasColor() { return false; }
	static constexpr bool hasTexture() { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;
	float3 normal;

	static const u32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalColor {
	VertexPositionNormalColor() = default;

	VertexPositionNormalColor(const float3& position, const float3& normal, const float4& color)
		: position(position)
		, normal(normal)
		, color(color) {
	}

	bool operator==(const VertexPositionNormalColor& compare) {
		return (this->position == compare.position &&
		        this->normal == compare.normal &&
		        this->color == compare.color);
	}

	bool operator!=(const VertexPositionNormalColor& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return true; }
	static constexpr bool hasColor() { return true; }
	static constexpr bool hasTexture() { return false; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;
	float3 normal;
	float4 color;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalTexture {
	VertexPositionNormalTexture() = default;

	VertexPositionNormalTexture(const float3& position, const float3& normal, const float2& texCoord)
		: position(position)
		, normal(normal)
		, texCoord(texCoord) {
	}

	bool operator==(const VertexPositionNormalTexture& compare) {
		return (this->position == compare.position &&
		        this->normal == compare.normal &&
		        this->texCoord == compare.texCoord);
	}

	bool operator!=(const VertexPositionNormalTexture& compare) {
		return !(*this == compare);
	}

	static constexpr bool hasNormal() { return true; }
	static constexpr bool hasColor() { return false; }
	static constexpr bool hasTexture() { return true; }


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	float3 position;
	float3 normal;
	float2 texCoord;

	static const u32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

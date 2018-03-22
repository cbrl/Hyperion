#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"


using namespace DirectX;


struct VertexPosition {
	VertexPosition() = default;

	VertexPosition(const float3& position)
		: position(position)
	{}

	VertexPosition(FXMVECTOR position) {
		XMStoreFloat3(&this->position, position);
	}

	bool operator==(const VertexPosition& compare) {
		return (this->position == compare.position);
	}

	bool operator!=(const VertexPosition& compare) {
		return !(*this == compare);
	}

	float3 position;

	static const i32 InputElementCount = 1;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionColor {
	VertexPositionColor() = default;

	VertexPositionColor(const float3& position, const float4& color)
		: position(position)
		, color(color)
	{}

	VertexPositionColor(FXMVECTOR position, FXMVECTOR color) {
		XMStoreFloat3(&this->position, position);
		XMStoreFloat4(&this->color, color);
	}

	bool operator==(const VertexPositionColor& compare) {
		return (this->position == compare.position &&
				this->color    == compare.color);
	}

	bool operator!=(const VertexPositionColor& compare) {
		return !(*this == compare);
	}

	float3 position;
	float4 color;

	static const i32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionTexture {
	VertexPositionTexture() = default;

	VertexPositionTexture(const float3& position, const float2& texCoord)
		: position(position)
		, texCoord(texCoord)
	{}

	VertexPositionTexture(FXMVECTOR position, FXMVECTOR texCoord) {
		XMStoreFloat3(&this->position, position);
		XMStoreFloat2(&this->texCoord, texCoord);
	}

	bool operator==(const VertexPositionTexture& compare) {
		return (this->position == compare.position &&
				this->texCoord == compare.texCoord);
	}

	bool operator!=(const VertexPositionTexture& compare) {
		return !(*this == compare);
	}

	float3 position;
	float2 texCoord;

	static const i32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionDualTexture {
	VertexPositionDualTexture() = default;

	VertexPositionDualTexture(const float3& position, const float2& texCoord0, const float2& texCoord1)
		: position(position)
		, texCoord0(texCoord0)
		, texCoord1(texCoord1)
	{}

	VertexPositionDualTexture(FXMVECTOR position,
	                          FXMVECTOR texCoord0,
							  FXMVECTOR texCoord1) {
		XMStoreFloat3(&this->position,  position);
		XMStoreFloat2(&this->texCoord0, texCoord0);
		XMStoreFloat2(&this->texCoord1, texCoord1);
	}

	bool operator==(const VertexPositionDualTexture& compare) {
		return (this->position  == compare.position  &&
				this->texCoord0 == compare.texCoord0 &&
				this->texCoord1 == compare.texCoord1);
	}

	bool operator!=(const VertexPositionDualTexture& compare) {
		return !(*this == compare);
	}

	float3 position;
	float2 texCoord0;
	float2 texCoord1;

	static const i32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormal {
	VertexPositionNormal() = default;

	VertexPositionNormal(const float3& position, const float3& normal)
		: position(position)
		, normal(normal)
	{}

	VertexPositionNormal(FXMVECTOR position, FXMVECTOR normal) {
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal,   normal);
	}

	bool operator==(const VertexPositionNormal& compare) {
		return (this->position == compare.position &&
				this->normal   == compare.normal);
	}

	bool operator!=(const VertexPositionNormal& compare) {
		return !(*this == compare);
	}

	float3 position;
	float3 normal;

	static const i32 InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalColor {
	VertexPositionNormalColor() = default;

	VertexPositionNormalColor(const float3& position, const float3& normal, const float4& color)
		: position(position)
		, normal(normal)
		, color(color)
	{}

	VertexPositionNormalColor(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color) {
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal,   normal);
		XMStoreFloat4(&this->color,    color);
	}

	bool operator==(const VertexPositionNormalColor& compare) {
		return (this->position == compare.position &&
				this->normal   == compare.normal   &&
				this->color    == compare.color);
	}

	bool operator!=(const VertexPositionNormalColor& compare) {
		return !(*this == compare);
	}

	float3 position;
	float3 normal;
	float4 color;

	static const i32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalTexture {
	VertexPositionNormalTexture() = default;

	VertexPositionNormalTexture(const float3& position, const float3& normal, const float2& texCoord)
		: position(position)
		, normal(normal)
		, texCoord(texCoord)
	{}

	VertexPositionNormalTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR texCoord) {
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal,   normal);
		XMStoreFloat2(&this->texCoord, texCoord);
	}

	bool operator==(const VertexPositionNormalTexture& compare) {
		return (this->position == compare.position &&
				this->normal   == compare.normal   &&
				this->texCoord == compare.texCoord);
	}

	bool operator!=(const VertexPositionNormalTexture& compare) {
		return !(*this == compare);
	}

	float3 position;
	float3 normal;
	float2 texCoord;

	static const i32 InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};



// Check if vertex type has a normal
template <typename T, typename = int>
struct HasNormal : std::false_type {};

template <typename T>
struct HasNormal <T, decltype((void)T::normal, 0)> : std::true_type {};



// Check if vertex type has a texCoord
template <typename T, typename = int>
struct HasTexCoord : std::false_type {};

template <typename T>
struct HasTexCoord <T, decltype((void)T::texCoord, 0)> : std::true_type {};


// Check if vertex type has a color
template <typename T, typename = int>
struct HasColor : std::false_type {};

template <typename T>
struct HasColor <T, decltype((void)T::color, 0)> : std::true_type {};
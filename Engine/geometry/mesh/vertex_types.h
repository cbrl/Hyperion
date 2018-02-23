#pragma once

#include <d3d11.h>
#include "util\math\math.h"


using namespace DirectX;


struct VertexPosition {
	VertexPosition() = default;

	VertexPosition(const XMFLOAT3& position)
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

	XMFLOAT3 position;

	static const int InputElementCount = 1;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionColor {
	VertexPositionColor() = default;

	VertexPositionColor(const XMFLOAT3& position, const XMFLOAT4& color)
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

	XMFLOAT3 position;
	XMFLOAT4 color;

	static const int InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionTexture {
	VertexPositionTexture() = default;

	VertexPositionTexture(const XMFLOAT3& position, const XMFLOAT2& texCoord)
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

	XMFLOAT3 position;
	XMFLOAT2 texCoord;

	static const int InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionDualTexture {
	VertexPositionDualTexture() = default;

	VertexPositionDualTexture(const XMFLOAT3& position, const XMFLOAT2& texCoord0, const XMFLOAT2& texCoord1)
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

	XMFLOAT3 position;
	XMFLOAT2 texCoord0;
	XMFLOAT2 texCoord1;

	static const int InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormal {
	VertexPositionNormal() = default;

	VertexPositionNormal(const XMFLOAT3& position, const XMFLOAT3& normal)
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

	XMFLOAT3 position;
	XMFLOAT3 normal;

	static const int InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalColor {
	VertexPositionNormalColor() = default;

	VertexPositionNormalColor(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT4& color)
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

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;

	static const int InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};


struct VertexPositionNormalTexture {
	VertexPositionNormalTexture() = default;

	VertexPositionNormalTexture(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT2& texCoord)
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

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;

	static const int InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

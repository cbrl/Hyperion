#include "vertex_types.h"
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;


// VertexPosition
const D3D11_INPUT_ELEMENT_DESC VertexPosition::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPosition) == 12, "Vertex struct/layout mismatch");


// VertexPositionColor
const D3D11_INPUT_ELEMENT_DESC VertexPositionColor::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionColor) == 28, "Vertex struct/layout mismatch");


// VertexPositionTexture
const D3D11_INPUT_ELEMENT_DESC VertexPositionTexture::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionTexture) == 20, "Vertex struct/layout mismatch");


// VertexPositionDualTexture
const D3D11_INPUT_ELEMENT_DESC VertexPositionDualTexture::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionDualTexture) == 28, "Vertex struct/layout mismatch");


// VertexPositionNormal
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormal::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormal) == 24, "Vertex struct/layout mismatch");


// VertexPositionNormalColor
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColor::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormalColor) == 40, "Vertex struct/layout mismatch");


// VertexPositionNormalTexture
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTexture::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormalTexture) == 32, "Vertex struct/layout mismatch");
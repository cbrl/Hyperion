#pragma once

#include "directx/d3d11.h"
#include "datatypes/vector_types.h"


//----------------------------------------------------------------------------------
// Position
//----------------------------------------------------------------------------------
struct VertexPosition final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr VertexPosition() noexcept = default;
	constexpr VertexPosition(const VertexPosition& vertex) noexcept = default;
	constexpr VertexPosition(VertexPosition&& vertex) noexcept = default;

	constexpr VertexPosition(const f32_3& position) noexcept
		: position(position) {
	}

	
	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPosition() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPosition& operator=(const VertexPosition& vertex) noexcept = default;
	constexpr VertexPosition& operator=(VertexPosition&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;

	static constexpr u32 input_element_count = 1;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPosition) == 12, "Vertex struct/layout mismatch");




//----------------------------------------------------------------------------------
// Position/Color
//----------------------------------------------------------------------------------
struct VertexPositionColor final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr VertexPositionColor() noexcept = default;
	constexpr VertexPositionColor(const VertexPositionColor& vertex) noexcept = default;
	constexpr VertexPositionColor(VertexPositionColor&& vertex) noexcept = default;

	constexpr VertexPositionColor(const f32_3& position, const f32_4& color) noexcept
		: position(position)
		, color(color) {
	}


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPositionColor() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPositionColor& operator=(const VertexPositionColor& vertex) noexcept = default;
	constexpr VertexPositionColor& operator=(VertexPositionColor&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return true; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_4 color;

	static constexpr u32 input_element_count = 2;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPositionColor) == 28, "Vertex struct/layout mismatch");




//----------------------------------------------------------------------------------
// Position/Texture
//----------------------------------------------------------------------------------
struct VertexPositionTexture final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr VertexPositionTexture() noexcept = default;
	constexpr VertexPositionTexture(const VertexPositionTexture& vertex) noexcept = default;
	constexpr VertexPositionTexture(VertexPositionTexture&& vertex) noexcept = default;

	constexpr VertexPositionTexture(const f32_3& position, const f32_2& texCoord) noexcept
		: position(position)
		, texCoord(texCoord) {
	}


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPositionTexture() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPositionTexture& operator=(const VertexPositionTexture& vertex) noexcept = default;
	constexpr VertexPositionTexture& operator=(VertexPositionTexture&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_2 texCoord;

	static constexpr u32 input_element_count = 2;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPositionColor) == 28, "Vertex struct/layout mismatch");




//----------------------------------------------------------------------------------
// Position/Texture/Texture
//----------------------------------------------------------------------------------
struct VertexPositionDualTexture final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
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


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPositionDualTexture() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPositionDualTexture& operator=(const VertexPositionDualTexture& vertex) noexcept = default;
	constexpr VertexPositionDualTexture& operator=(VertexPositionDualTexture&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return false; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_2 texCoord0;
	f32_2 texCoord1;

	static constexpr u32 InputElementCount = 3;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[InputElementCount] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPositionDualTexture) == 28, "Vertex struct/layout mismatch");




//----------------------------------------------------------------------------------
// Position/Normal
//----------------------------------------------------------------------------------
struct VertexPositionNormal final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr VertexPositionNormal() noexcept = default;
	constexpr VertexPositionNormal(const VertexPositionNormal& vertex) noexcept = default;
	constexpr VertexPositionNormal(VertexPositionNormal&& vertex) noexcept = default;

	constexpr VertexPositionNormal(const f32_3& position, const f32_3& normal) noexcept
		: position(position)
		, normal(normal) {
	}


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPositionNormal() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPositionNormal& operator=(const VertexPositionNormal& vertex) noexcept = default;
	constexpr VertexPositionNormal& operator=(VertexPositionNormal&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_3 normal;

	static constexpr u32 input_element_count = 2;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPositionNormal) == 24, "Vertex struct/layout mismatch");




//----------------------------------------------------------------------------------
// Position/Normal/Color
//----------------------------------------------------------------------------------
struct VertexPositionNormalColor final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
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


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPositionNormalColor() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPositionNormalColor& operator=(const VertexPositionNormalColor& vertex) noexcept = default;
	constexpr VertexPositionNormalColor& operator=(VertexPositionNormalColor&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return true; }
	static constexpr bool hasTexture() noexcept { return false; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_3 normal;
	f32_4 color;

	static constexpr u32 input_element_count = 3;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPositionNormalColor) == 40, "Vertex struct/layout mismatch");




//----------------------------------------------------------------------------------
// Position/Normal/Texture
//----------------------------------------------------------------------------------
struct VertexPositionNormalTexture final {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
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


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~VertexPositionNormalTexture() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr VertexPositionNormalTexture& operator=(const VertexPositionNormalTexture& vertex) noexcept = default;
	constexpr VertexPositionNormalTexture& operator=(VertexPositionNormalTexture&& vertex) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	static constexpr bool hasNormal() noexcept { return true; }
	static constexpr bool hasColor() noexcept { return false; }
	static constexpr bool hasTexture() noexcept { return true; }


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 position;
	f32_3 normal;
	f32_2 texCoord;

	static constexpr u32 input_element_count = 3;
	static constexpr D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

static_assert(sizeof(VertexPositionNormalTexture) == 32, "Vertex struct/layout mismatch");

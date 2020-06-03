#pragma once

#include "directx/d3d11.h"
#include "datatypes/vector_types.h"


namespace render {

class Viewport final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Viewport() noexcept = default;

	explicit Viewport(f32 width, f32 height) noexcept {
		viewport.Width = width;
		viewport.Height = height;
	}

	explicit Viewport(f32_2 size) noexcept {
		viewport.Width = size[0];
		viewport.Height = size[1];
	}

	Viewport(const Viewport& copy) noexcept = default;
	Viewport(Viewport&& move) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Viewport() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Viewport& operator=(const Viewport& copy) noexcept = default;
	Viewport& operator=(Viewport&& move) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Bind
	//----------------------------------------------------------------------------------

	void bind(ID3D11DeviceContext& device_context) const noexcept {
		Pipeline::RS::bindViewports(device_context, std::span{&viewport, 1});
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Viewport
	//----------------------------------------------------------------------------------

	const D3D11_VIEWPORT& getViewport() const noexcept {
		return viewport;
	}

	void setViewport(D3D11_VIEWPORT vp) noexcept {
		viewport = std::move(vp);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Top Left
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32_2 getTopLeft() const noexcept {
		return u32_2{ static_cast<u32>(viewport.TopLeftX),
		                 static_cast<u32>(viewport.TopLeftY) };
	}

	void setTopLeft(u32 x, u32 y) noexcept {
		viewport.TopLeftX = static_cast<f32>(x);
		viewport.TopLeftY = static_cast<f32>(y);
	}

	void setTopLeft(const u32_2& top_left) noexcept {
		viewport.TopLeftX = static_cast<f32>(top_left[0]);
		viewport.TopLeftY = static_cast<f32>(top_left[1]);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Size
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32_2 getSize() const noexcept {
		return u32_2{ static_cast<u32>(viewport.Width),
		                 static_cast<u32>(viewport.Height) };
	}

	void setSize(u32 width, u32 height) noexcept {
		viewport.Width  = static_cast<f32>(width);
		viewport.Height = static_cast<f32>(height);
	}

	void setSize(const u32_2& size) noexcept {
		viewport.Width  = static_cast<f32>(size[0]);
		viewport.Height = static_cast<f32>(size[1]);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Depth
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	f32_2 getDepth() const noexcept {
		return f32_2{ viewport.MinDepth, viewport.MaxDepth };
	}

	void setDepth(f32 min, f32 max) noexcept {
		viewport.MinDepth = min;
		viewport.MaxDepth = max;
	}

	void setDepth(const f32_2& depth_range) noexcept {
		viewport.MinDepth = depth_range[0];
		viewport.MaxDepth = depth_range[1];
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Aspect Ratio
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32 getAspectRatio() const noexcept {
		return static_cast<f32>(viewport.Width) / static_cast<f32>(viewport.Height);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Contains Point
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(u32 x, u32 y) const noexcept {
		const u32_2 bounds = {
			viewport.Width  + viewport.TopLeftX,
			viewport.Height + viewport.TopLeftY
		};

		if (x >= viewport.TopLeftX &&
			y >= viewport.TopLeftY &&
			x <= bounds[0]         &&
			y <= bounds[1]) {
			return true;
		}

		return false;
	}

	[[nodiscard]]
	bool contains(const u32_2& pos) const noexcept {
		return contains(pos[0], pos[1]);
	}


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	D3D11_VIEWPORT viewport = {};
};

} //namespace render

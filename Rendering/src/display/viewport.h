#pragma once

#include "directx/d3d11.h"
#include "pipeline.h"
#include "datatypes/vector_types.h"


class Viewport final {
public:
	Viewport() noexcept : viewport{} {}
	Viewport(const Viewport& copy) noexcept = default;
	Viewport(Viewport&& move) noexcept = default;

	explicit Viewport(f32 width, f32 height) noexcept
		: viewport{} {
		viewport.Width  = width;
		viewport.Height = height;
	}

	explicit Viewport(f32_2 size) noexcept
		: viewport{} {
		viewport.Width  = size.x;
		viewport.Height = size.y;
	}

	~Viewport() = default;

	Viewport& operator=(const Viewport& copy) noexcept = default;
	Viewport& operator=(Viewport&& move) noexcept = default;


	//----------------------------------------------------------------------------------
	// Bind
	//----------------------------------------------------------------------------------

	void bind(ID3D11DeviceContext& device_context) const noexcept {
		Pipeline::RS::bindViewports(device_context, 1, &viewport);
	}


	//----------------------------------------------------------------------------------
	// Viewport
	//----------------------------------------------------------------------------------

	const D3D11_VIEWPORT& getViewport() const noexcept {
		return viewport;
	}

	void setViewport(D3D11_VIEWPORT vp) noexcept {
		viewport = std::move(vp);
	}


	//----------------------------------------------------------------------------------
	// Top Left
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

	void setTopLeft(u32_2 top_left) noexcept {
		viewport.TopLeftX = static_cast<f32>(top_left.x);
		viewport.TopLeftY = static_cast<f32>(top_left.y);
	}


	//----------------------------------------------------------------------------------
	// Size
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

	void setSize(u32_2 size) noexcept {
		viewport.Width  = static_cast<f32>(size.x);
		viewport.Height = static_cast<f32>(size.y);
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	f32_2 getDepth() const noexcept {
		return f32_2{ viewport.MinDepth, viewport.MaxDepth };
	}

	void setDepth(f32 min, f32 max) noexcept {
		viewport.MinDepth = min;
		viewport.MaxDepth = max;
	}

	void setDepth(f32_2 depth_range) noexcept {
		viewport.MinDepth = depth_range.x;
		viewport.MaxDepth = depth_range.y;
	}


private:
	D3D11_VIEWPORT viewport;
};

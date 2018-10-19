#pragma once

#include "datatypes/datatypes.h"


class RenderingConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	RenderingConfig() = default;
	RenderingConfig(const RenderingConfig& config) noexcept = default;
	RenderingConfig(RenderingConfig&& config) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~RenderingConfig() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	RenderingConfig& operator=(const RenderingConfig& config) noexcept = default;
	RenderingConfig& operator=(RenderingConfig&& config) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void setShadowMapRes(u32 res) noexcept {
		smap_res = (res == 0) ? 1 : res;
	}

	[[nodiscard]]
	u32 getShadowMapRes() const noexcept {
		return smap_res;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	u32 smap_res;
};
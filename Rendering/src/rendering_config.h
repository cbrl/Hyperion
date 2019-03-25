#pragma once

#include "datatypes/scalar_types.h"
#include "config/config_tokens.h"
#include "json/json.h"


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

	// Shadow maps are square, so only one value is needed. The resolution will not be
	// changed if an invalid value of 0 is provided.
	void setShadowMapRes(u32 res) noexcept {
		if (res != 0) smap_res = res;
	}

	[[nodiscard]]
	u32 getShadowMapRes() const noexcept {
		return smap_res;
	}


	//----------------------------------------------------------------------------------
	// Friend Functions - JSON Serialization
	//----------------------------------------------------------------------------------
	friend void to_json(json& j, const RenderingConfig& cfg) {
		j[ConfigTokens::shadowmap_res] = cfg.smap_res;
	}

	friend void from_json(const json& j, RenderingConfig& cfg) {
		const auto res = j.at(ConfigTokens::shadowmap_res).get<u32>();
		cfg.setShadowMapRes(res);
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	u32 smap_res = 512;
};
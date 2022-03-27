#pragma once

#include "datatypes/scalar_types.h"
#include "engine/config/config_tokens.h"
#include "json/nlohmann_json.h"


namespace render {

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

	// Shadow maps are square, so only one value is needed. The provided resolution must
	// be greater than 0, or no change will occur.
	void setShadowMapRes(u32 res) noexcept {
		if (res != 0) smap_res = res;
	}

	[[nodiscard]]
	u32 getShadowMapRes() const noexcept {
		return smap_res;
	}

	void setShadowMapDepthBias(i32 depth_bias) noexcept {
		smap_depth_bias = depth_bias;
	}

	[[nodiscard]]
	i32 getShadowMapDepthBias() const noexcept {
		return smap_depth_bias;
	}

	void setShadowMapSlopeScaledDepthBias(f32 slope_scaled_depth_bias) noexcept {
		smap_slope_scaled_depth_bias = slope_scaled_depth_bias;
	}

	[[nodiscard]]
	f32 getShadowMapSlopeScaledDepthBias() const noexcept {
		return smap_slope_scaled_depth_bias;
	}

	void setShadowMapDepthBiasClamp(f32 depth_bias_clamp) noexcept {
		smap_depth_bias_clamp = depth_bias_clamp;
	}

	[[nodiscard]]
	f32 getShadowMapDepthBiasClamp() const noexcept {
		return smap_depth_bias_clamp;
	}


	//----------------------------------------------------------------------------------
	// Friend Functions - JSON Serialization
	//----------------------------------------------------------------------------------
	friend void to_json(nl::json& j, const RenderingConfig& cfg) {
		j[ConfigTokens::smap_res]                     = cfg.smap_res;
		j[ConfigTokens::smap_depth_bias]              = cfg.smap_depth_bias;
		j[ConfigTokens::smap_slope_scaled_depth_bias] = cfg.smap_slope_scaled_depth_bias;
		j[ConfigTokens::smap_depth_bias_clamp]        = cfg.smap_depth_bias_clamp;
	}

	friend void from_json(const nl::json& j, RenderingConfig& cfg) {
		if (j.contains(ConfigTokens::smap_res)) {
			const auto res = j.at(ConfigTokens::smap_res).get<u32>();
			cfg.setShadowMapRes(res);
		}

		if (j.contains(ConfigTokens::smap_depth_bias))
			j.at(ConfigTokens::smap_depth_bias).get_to(cfg.smap_depth_bias);

		if (j.contains(ConfigTokens::smap_slope_scaled_depth_bias))
			j.at(ConfigTokens::smap_slope_scaled_depth_bias).get_to(cfg.smap_slope_scaled_depth_bias);

		if (j.contains(ConfigTokens::smap_depth_bias_clamp))
			j.at(ConfigTokens::smap_depth_bias_clamp).get_to(cfg.smap_depth_bias_clamp);
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	u32 smap_res                     = 512;
	i32 smap_depth_bias              = 50;
	f32 smap_slope_scaled_depth_bias = 1.0f;
	f32 smap_depth_bias_clamp        = 0.0f;
};

} //namespace render
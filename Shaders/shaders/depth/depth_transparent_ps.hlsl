#include "forward/forward_include.hlsli"


void PS(PSPositionTexture pin) {
	
	float alpha = 1.0f;

	if (g_material.has_texture) {
		alpha  = diffuse_map.Sample(g_linear_wrap, pin.uv).w;
		alpha *= g_material.opacity;
	}
	else {
		alpha = g_material.opacity;
	}

	clip(alpha - ALPHA_SHADOW_THRESHOLD);
}
#include "forward/forward_include.hlsli"


void PS(PSTexture pin) {
	
	float alpha = 1.0f;

	if (g_material.has_texture) {
		alpha = diffuse_map.Sample(g_linear_wrap, pin.tex).w;
		alpha *= g_material.diffuse.w;
	}
	else {
		alpha = g_material.diffuse.w;
	}

	clip(alpha - ALPHA_SHADOW_THRESHOLD);
}
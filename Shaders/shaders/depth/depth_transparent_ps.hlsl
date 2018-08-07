#include "forward/forward_include.hlsli"


void PS(PSTexture pin) {
	
	float alpha = 1.0f;

	if (mat.has_texture) {
		alpha = diffuse_map.Sample(linear_wrap, pin.tex).w;
		alpha *= mat.diffuse.w;
	}
	else {
		alpha = mat.diffuse.w;
	}

	clip(alpha - ALPHA_SHADOW_THRESHOLD);
}
#include "forward/forward_include.hlsli"


void PS(PSPositionTexture pin) {
	
	const float4 base_color = GetBaseColor(pin.uv);
	clip(base_color.w - ALPHA_SHADOW_THRESHOLD);
}
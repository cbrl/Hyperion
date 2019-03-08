#include "forward/forward_include.hlsli"


OMGBuffer PS(PSPositionNormalTexture pin) {

	const float4 base_color = GetBaseColor(pin.uv);
	clip(base_color.w - ALPHA_MIN);

	const float2 material_params = GetMaterialParams(pin.uv);
	const float3 normal = GetNormal(pin.p_world, pin.n, pin.uv);

	OMGBuffer pout;
	pout.base_color = base_color;
	pout.material   = float4(material_params, 0.0f, 1.0f);
	pout.n          = float4(normal, 1.0f);

	return pout;
}
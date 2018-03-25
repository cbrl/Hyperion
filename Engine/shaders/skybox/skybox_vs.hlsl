#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"


CONSTANT_BUFFER(WVP, SLOT_CBUFFER_WVP) {
	matrix world_view_proj;
}


PSPosition VS(VSPosition vin) {
	PSPosition vout;

	vout.position = mul(float4(vin.position, 1.0f), world_view_proj).xyww;

	vout.w_position = vin.position;

	return vout;
}
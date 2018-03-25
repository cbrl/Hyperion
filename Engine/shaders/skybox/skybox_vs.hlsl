#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"


PSPosition VS(VSPosition vin) {
	PSPosition vout;

	vout.position = mul(float4(vin.position, 1.0f), camera_wvp).xyww;

	vout.w_position = vin.position;

	return vout;
}
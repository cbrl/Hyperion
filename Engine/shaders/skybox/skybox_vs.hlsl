#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"


PSPosition VS(VSPosition vin) {
	PSPosition vout;

	const float3 p = mul(float4(vin.position, 1.0f), world_to_camera).xyz;
	vout.position = mul(float4(p, 1.0f), camera_to_projection).xyww;

	//vout.position = mul(float4(vin.position, 1.0f), camera_wvp).xyww;

	vout.w_position = vin.position;

	return vout;
}
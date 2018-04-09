#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"


TEXTURE_CUBE(skybox, SLOT_SRV_SKYBOX);


float4 PS(PSPosition pin) : SV_Target {
	return skybox.Sample(linear_wrap, pin.w_position);
}
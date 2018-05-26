#include "include\global.hlsl"
#include "include\input_structs.hlsl"


TEXTURE_CUBE(skybox, SLOT_SRV_SKYBOX);


float4 PS(PSPosition pin) : SV_Target {
	return skybox.Sample(linear_wrap, pin.position_world);
}
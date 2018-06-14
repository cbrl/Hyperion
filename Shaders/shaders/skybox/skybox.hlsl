#include "include\global.hlsl"
#include "include\input_structs.hlsl"


TextureCube skybox : REG_T(SLOT_SRV_SKYBOX);


float4 PS(PSPosition pin) : SV_Target {
	return skybox.Sample(linear_wrap, pin.position_world);
}
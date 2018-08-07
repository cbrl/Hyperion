#include "include/global.hlsli"


TextureCube skybox : REG_T(SLOT_SRV_SKYBOX);


float4 PS(PSWorldPosition pin) : SV_Target {
	return skybox.Sample(linear_wrap, pin.position_world);
}
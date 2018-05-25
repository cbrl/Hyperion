#ifndef HLSL_TRANSFORM
#define HLSL_TRANSFORM


// Transform a point from object space to projection space
float4 Transform(float3 position,
				 matrix object_to_world,
				 matrix world_to_view,
				 matrix view_to_projection) {

	float3 world = mul(float4(position, 1.0f), object_to_world).xyz;
	float3 view = mul(float4(world, 1.0f), world_to_view).xyz;
	float4 proj = mul(float4(view, 1.0f), view_to_projection);

	return proj;
}


// Transform a point from world space to projection space
float4 Transform(float3 world,
				 matrix world_to_view,
				 matrix view_to_projection) {

	const float3 view = mul(float4(world, 1.0f), world_to_view).xyz;
	const float4 proj = mul(float4(view, 1.0f), view_to_projection);

	return proj;
}


#endif //HLSL_TRANSFORM
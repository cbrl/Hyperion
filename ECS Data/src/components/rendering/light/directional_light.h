#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


class DirectionalLight final : public Component<DirectionalLight> {
public:
	DirectionalLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 0.0f)
		, diffuse_color(1.0f, 1.0f, 1.0f, 0.0f)
		, specular(1.0f, 1.0f, 1.0f, 1.0f)
		, start(0.1f)
		, range(1.0f)
		, proj_size(1.0f, 1.0f)
		, shadows(false) {
	}

	~DirectionalLight() = default;

	void setAmbientColor(const float4& color) {
		this->ambient_color = color;
	}

	void setDiffuseColor(const float4& color) {
		this->diffuse_color = color;
	}

	void setSpecular(const float4& spec) {
		this->specular = spec;
	}

	void setShadows(bool state) {
		shadows = state;
	}

	void setRange(float range) {
		this->range = range;
		updateBoundingVolumes();
	}

	void setSize(const float2& size) {
		proj_size = size;
		updateBoundingVolumes();
	}


	const float4& getAmbientColor() const {
		return ambient_color;
	}

	const float4& getDiffuseColor() const {
		return diffuse_color;
	}

	const float4& getSpecular() const {
		return specular;
	}

	bool castsShadows() const {
		return shadows;
	}

	float getRange() const {
		return range;
	}

	const float2& getSize() const {
		return proj_size;
	}

	const AABB& getAABB() const {
		return aabb;
	}

	XMMATRIX XM_CALLCONV getLightToProjectionMatrix() const {
		return XMMatrixOrthographicLH(proj_size.x, proj_size.y, start, range);
	}


private:
	void updateBoundingVolumes() {
		aabb = AABB{ float3{-0.5f * proj_size.x, -0.5f * proj_size.y, 0.0f},
					 float3{ 0.5f * proj_size.x,  0.5f * proj_size.y, range} };
	}


private:
	float4 ambient_color;
	float4 diffuse_color;
	float4 specular;
	float start;
	float range;
	float2 proj_size;
	AABB aabb;

	bool shadows;
};

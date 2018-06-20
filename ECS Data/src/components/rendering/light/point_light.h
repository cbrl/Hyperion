#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


class PointLight final : public Component<PointLight> {
public:
	PointLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 0.0f)
		, diffuse_color(1.0f, 1.0f, 1.0f, 0.0f)
		, specular(1.0f, 1.0f, 1.0f, 1.0f)
		, attenuation(0.0f, 1.0f, 0.0f)
		, near_plane(0.1f)
		, range(1.0f)
		, shadows(false) {
	}

	~PointLight() = default;


	void setAmbientColor(const float4& color) {
		this->ambient_color = color;
	}

	void setDiffuseColor(const float4& color) {
		this->diffuse_color = color;
	}

	void setSpecular(const float4& spec) {
		this->specular = spec;
	}

	void setRange(const float range) {
		this->range = range;
		updateBoundingVolumes();
	}

	void setAttenuation(const float3& attenuation) {
		this->attenuation = attenuation;
	}

	void setShadows(bool state) {
		this->shadows = state;
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

	const float3& getAttenuation() const {
		return attenuation;
	}

	float getRange() const {
		return range;
	}

	bool castsShadows() const {
		return shadows;
	}

	const AABB& getAABB() const {
		return aabb;
	}

	const BoundingSphere& getBoundingSphere() const {
		return sphere;
	}

	XMMATRIX XM_CALLCONV getLightToProjectionMatrix() const {

		const float m22 = range / (range - near_plane);
		const float m32 = -near_plane * m22;

		return XMMATRIX{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f,  m22, 1.0f,
			0.0f, 0.0f,  m32, 0.0f
		};
	}


private:
	void updateBoundingVolumes() {
		aabb   = AABB(float3(-range, -range, -range), float3(range, range, range));
		sphere = BoundingSphere(float3(0.0f, 0.0f, 0.0f), range);
	}


private:
	// Lighting parameters
	float4 ambient_color;
	float4 diffuse_color;
	float4 specular;
	float3 attenuation;

	// Near clipping plane and range (far plane)
	float near_plane;
	float range;

	// Flag that decides if the light should cast shadows
	bool shadows;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};

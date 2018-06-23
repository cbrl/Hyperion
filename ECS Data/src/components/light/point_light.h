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


	//----------------------------------------------------------------------------------
	// Ambient Color
	//----------------------------------------------------------------------------------
	void setAmbientColor(const f32_4& color) {
		this->ambient_color = color;
	}

	[[nodiscard]]
	const f32_4& getAmbientColor() const {
		return ambient_color;
	}


	//----------------------------------------------------------------------------------
	// Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const f32_4& color) {
		this->diffuse_color = color;
	}

	[[nodiscard]]
	const f32_4& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const f32_4& spec) {
		this->specular = spec;
	}

	[[nodiscard]]
	const f32_4& getSpecular() const {
		return specular;
	}


	//----------------------------------------------------------------------------------
	// Range
	//----------------------------------------------------------------------------------
	void setRange(const float range) {
		this->range = range;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	float getRange() const {
		return range;
	}


	//----------------------------------------------------------------------------------
	// Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(const f32_3& attenuation) {
		this->attenuation = attenuation;
	}

	[[nodiscard]]
	const f32_3& getAttenuation() const {
		return attenuation;
	}


	//----------------------------------------------------------------------------------
	// Shadows
	//----------------------------------------------------------------------------------
	void setShadows(bool state) {
		this->shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const {
		return shadows;
	}


	// Get the AABB of this light
	[[nodiscard]]
	const AABB& getAABB() const {
		return aabb;
	}

	// Get the bounding sphere of this light
	[[nodiscard]]
	const BoundingSphere& getBoundingSphere() const {
		return sphere;
	}


	// Get the light-to-projection matrix
	[[nodiscard]]
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
		aabb   = AABB(f32_3(-range, -range, -range), f32_3(range, range, range));
		sphere = BoundingSphere(f32_3(0.0f, 0.0f, 0.0f), range);
	}


private:
	// Lighting parameters
	f32_4 ambient_color;
	f32_4 diffuse_color;
	f32_4 specular;
	f32_3 attenuation;

	// Near clipping plane and range (far plane)
	float near_plane;
	float range;

	// Flag that decides if the light should cast shadows
	bool shadows;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};

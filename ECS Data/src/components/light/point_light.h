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
	void setAmbientColor(const vec4_f32& color) {
		this->ambient_color = color;
	}

	[[nodiscard]]
	const vec4_f32& getAmbientColor() const {
		return ambient_color;
	}


	//----------------------------------------------------------------------------------
	// Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const vec4_f32& color) {
		this->diffuse_color = color;
	}

	[[nodiscard]]
	const vec4_f32& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const vec4_f32& spec) {
		this->specular = spec;
	}

	[[nodiscard]]
	const vec4_f32& getSpecular() const {
		return specular;
	}


	//----------------------------------------------------------------------------------
	// Range
	//----------------------------------------------------------------------------------
	void setRange(const f32 range) {
		this->range = range;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	f32 getRange() const {
		return range;
	}


	//----------------------------------------------------------------------------------
	// Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(const vec3_f32& attenuation) {
		this->attenuation = attenuation;
	}

	[[nodiscard]]
	const vec3_f32& getAttenuation() const {
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

		const f32 m22 = range / (range - near_plane);
		const f32 m32 = -near_plane * m22;

		return XMMATRIX{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f,  m22, 1.0f,
			0.0f, 0.0f,  m32, 0.0f
		};
	}


private:
	void updateBoundingVolumes() {
		aabb   = AABB(vec3_f32(-range, -range, -range), vec3_f32(range, range, range));
		sphere = BoundingSphere(vec3_f32(0.0f, 0.0f, 0.0f), range);
	}


private:
	// Lighting parameters
	vec4_f32 ambient_color;
	vec4_f32 diffuse_color;
	vec4_f32 specular;
	vec3_f32 attenuation;

	// Near clipping plane and range (far plane)
	f32 near_plane;
	f32 range;

	// Flag that decides if the light should cast shadows
	bool shadows;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};

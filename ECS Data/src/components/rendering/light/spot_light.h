#pragma	once

#include "component/component.h"


class SpotLight final : public Component<SpotLight> {
public:
	SpotLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 0.0f)
		, diffuse_color(1.0f, 1.0f, 1.0f, 0.0f)
		, specular(1.0f, 1.0f, 1.0f, 1.0f)
		, attenuation(0.0f, 1.0f, 0.0f)
		, cos_umbra(1.0f)
		, cos_penumbra(0.1f)
		, near_plane(0.1f)
		, range(1.0f)
		, shadows(false) {
	}

	~SpotLight() = default;


	//----------------------------------------------------------------------------------
	// Ambient Color
	//----------------------------------------------------------------------------------
	void setAmbientColor(const float4& color) {
		ambient_color = color;
	}

	[[nodiscard]]
	const float4& getAmbientColor() const {
		return ambient_color;
	}


	//----------------------------------------------------------------------------------
	// Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const float4& color) {
		diffuse_color = color;
	}

	[[nodiscard]]
	const float4& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const float4& spec) {
		specular = spec;
	}

	[[nodiscard]]
	const float4& getSpecular() const {
		return specular;
	}


	//----------------------------------------------------------------------------------
	// Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(const float3& atten) {
		attenuation = atten;
	}

	[[nodiscard]]
	const float3& getAttenuation() const {
		return attenuation;
	}


	//----------------------------------------------------------------------------------
	// Umbra
	//----------------------------------------------------------------------------------
	void setUmbraCosAngle(float cos_angle) {
		cos_umbra = std::max(std::max(cos_angle, cos_penumbra + 0.001f), 0.001f);
	}

	void setUmbraAngle(float angle) {
		setUmbraCosAngle(std::cos(angle));
	}

	[[nodiscard]]
	float getUmbra() const {
		return cos_umbra;
	}

	[[nodiscard]]
	float getUmbraAngle() const {
		return std::acos(cos_umbra);
	}


	//----------------------------------------------------------------------------------
	// Penumbra
	//----------------------------------------------------------------------------------
	void setPenumbraCosAngle(float cos_angle) {
		cos_penumbra = std::max(std::min(cos_angle, cos_umbra - 0.001f), 0.001f);
		updateBoundingVolumes();
	}

	void setPenumbraAngle(float angle) {
		setPenumbraCosAngle(std::cos(angle));
	}

	[[nodiscard]]
	float getPenumbra() const {
		return cos_penumbra;
	}

	[[nodiscard]]
	float getPenumbraAngle() const {
		return std::acos(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Umbra/Penumbra
	//----------------------------------------------------------------------------------
	void setCutoffAngles(float cos_umbra, float cos_penumbra) {
		setUmbraAngle(cos_umbra);
		setPenumbraAngle(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Range
	//----------------------------------------------------------------------------------
	void setRange(float range) {
		this->range = range;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	float getRange() const {
		return range;
	}


	//----------------------------------------------------------------------------------
	// Shadows
	//----------------------------------------------------------------------------------
	void setShadows(bool state) {
		shadows = state;
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
		const float fov = std::acos(cos_penumbra) * 2.0f;
		return XMMatrixPerspectiveFovLH(fov, 1.0f, near_plane, range);
	}


private:
	void updateBoundingVolumes() {
		const float a   = 1.0f / (cos_penumbra * cos_penumbra);
		const float rxy = range * std::sqrt(a - 1.0f);
		const float rz  = range * 0.5f;
		const float r   = std::sqrt((rxy * rxy) + (rz * rz));

		aabb   = AABB(float3(-rxy, -rxy, 0.0f), float3(rxy, rxy, range));
		sphere = BoundingSphere(float3(0.0f, 0.0f, rz), r);
	}


private:
	// Lighting parameters
	float4 ambient_color;
	float4 diffuse_color;
	float4 specular;
	float3 attenuation;
	float cos_umbra;
	float cos_penumbra;

	// Near clipping plane and range (far plane)
	float near_plane;
	float range;

	// Flag that decides if the light should cast shadows
	bool shadows;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};

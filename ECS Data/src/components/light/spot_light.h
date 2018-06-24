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
	void setAmbientColor(const f32_4& color) {
		ambient_color = color;
	}

	[[nodiscard]]
	const f32_4& getAmbientColor() const {
		return ambient_color;
	}


	//----------------------------------------------------------------------------------
	// Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const f32_4& color) {
		diffuse_color = color;
	}

	[[nodiscard]]
	const f32_4& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const f32_4& spec) {
		specular = spec;
	}

	[[nodiscard]]
	const f32_4& getSpecular() const {
		return specular;
	}


	//----------------------------------------------------------------------------------
	// Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(const f32_3& atten) {
		attenuation = atten;
	}

	[[nodiscard]]
	const f32_3& getAttenuation() const {
		return attenuation;
	}


	//----------------------------------------------------------------------------------
	// Umbra
	//----------------------------------------------------------------------------------
	void setUmbraCosAngle(f32 cos_angle) {
		cos_umbra = std::max(std::max(cos_angle, cos_penumbra + 0.001f), 0.001f);
	}

	void setUmbraAngle(f32 angle) {
		setUmbraCosAngle(std::cos(angle));
	}

	[[nodiscard]]
	f32 getUmbra() const {
		return cos_umbra;
	}

	[[nodiscard]]
	f32 getUmbraAngle() const {
		return std::acos(cos_umbra);
	}


	//----------------------------------------------------------------------------------
	// Penumbra
	//----------------------------------------------------------------------------------
	void setPenumbraCosAngle(f32 cos_angle) {
		cos_penumbra = std::max(std::min(cos_angle, cos_umbra - 0.001f), 0.001f);
		updateBoundingVolumes();
	}

	void setPenumbraAngle(f32 angle) {
		setPenumbraCosAngle(std::cos(angle));
	}

	[[nodiscard]]
	f32 getPenumbra() const {
		return cos_penumbra;
	}

	[[nodiscard]]
	f32 getPenumbraAngle() const {
		return std::acos(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Umbra/Penumbra
	//----------------------------------------------------------------------------------
	void setCutoffAngles(f32 cos_umbra, f32 cos_penumbra) {
		setUmbraAngle(cos_umbra);
		setPenumbraAngle(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Range
	//----------------------------------------------------------------------------------
	void setRange(f32 range) {
		this->range = range;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	f32 getRange() const {
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
		const f32 fov = std::acos(cos_penumbra) * 2.0f;
		return XMMatrixPerspectiveFovLH(fov, 1.0f, near_plane, range);
	}


private:
	void updateBoundingVolumes() {
		const f32 a   = 1.0f / (cos_penumbra * cos_penumbra);
		const f32 rxy = range * std::sqrt(a - 1.0f);
		const f32 rz  = range * 0.5f;
		const f32 r   = std::sqrt((rxy * rxy) + (rz * rz));

		aabb   = AABB(f32_3(-rxy, -rxy, 0.0f), f32_3(rxy, rxy, range));
		sphere = BoundingSphere(f32_3(0.0f, 0.0f, rz), r);
	}


private:
	// Lighting parameters
	f32_4 ambient_color;
	f32_4 diffuse_color;
	f32_4 specular;
	f32_3 attenuation;
	f32 cos_umbra;
	f32 cos_penumbra;

	// Near clipping plane and range (far plane)
	f32 near_plane;
	f32 range;

	// Flag that decides if the light should cast shadows
	bool shadows;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};
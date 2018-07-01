#pragma	once

#include "component/component.h"


class SpotLight final : public Component<SpotLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SpotLight() noexcept
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

	SpotLight(const SpotLight& light) = delete;
	SpotLight(SpotLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SpotLight() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SpotLight& operator=(const SpotLight& light) = delete;
	SpotLight& operator=(SpotLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Ambient Color
	//----------------------------------------------------------------------------------
	void setAmbientColor(const vec4_f32& color) {
		ambient_color = color;
	}

	[[nodiscard]]
	const vec4_f32& getAmbientColor() const {
		return ambient_color;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const vec4_f32& color) {
		diffuse_color = color;
	}

	[[nodiscard]]
	const vec4_f32& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const vec4_f32& spec) {
		specular = spec;
	}

	[[nodiscard]]
	const vec4_f32& getSpecular() const {
		return specular;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(const vec3_f32& atten) {
		attenuation = atten;
	}

	[[nodiscard]]
	const vec3_f32& getAttenuation() const {
		return attenuation;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Umbra
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
	// Member Functions - Penumbra
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
	// Member Functions - Umbra/Penumbra
	//----------------------------------------------------------------------------------
	void setCutoffAngles(f32 cos_umbra, f32 cos_penumbra) {
		setUmbraAngle(cos_umbra);
		setPenumbraAngle(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Range
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
	// Member Functions - Shadows
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

		aabb   = AABB(vec3_f32(-rxy, -rxy, 0.0f), vec3_f32(rxy, rxy, range));
		sphere = BoundingSphere(vec3_f32(0.0f, 0.0f, rz), r);
	}


private:
	// Lighting parameters
	vec4_f32 ambient_color;
	vec4_f32 diffuse_color;
	vec4_f32 specular;
	vec3_f32 attenuation;
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

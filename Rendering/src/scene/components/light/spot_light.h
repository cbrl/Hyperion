#pragma	once

#include "component/component.h"


class SpotLight final : public Component<SpotLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SpotLight() noexcept
		: base_color(1.0f, 1.0f, 1.0f)
	    , intensity(1.0f)
		, attenuation(0.0f, 0.0f, 1.0f)
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
	// Member Functions - Base Color
	//----------------------------------------------------------------------------------
	void setBaseColor(f32_3 color) noexcept {
		base_color = std::move(color);
	}

	[[nodiscard]]
	f32_3& getBaseColor() noexcept {
		return base_color;
	}

	[[nodiscard]]
	const f32_3& getBaseColor() const noexcept {
		return base_color;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Intensity
	//----------------------------------------------------------------------------------
	void setIntensity(f32 value) noexcept {
		intensity = value;
	}

	[[nodiscard]] f32 getIntensity() const noexcept {
		return intensity;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(f32_3 atten) noexcept {
		attenuation = std::move(atten);
	}

	[[nodiscard]]
	f32_3& getAttenuation() noexcept {
		return attenuation;
	}

	[[nodiscard]]
	const f32_3& getAttenuation() const noexcept {
		return attenuation;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Umbra
	//----------------------------------------------------------------------------------
	void setUmbraCosAngle(f32 cos_angle) noexcept {
		cos_umbra = std::max(std::max(cos_angle, cos_penumbra + 0.001f), 0.001f);
	}

	void setUmbraAngle(f32 angle) noexcept {
		setUmbraCosAngle(std::cos(angle));
	}

	[[nodiscard]]
	f32 getUmbra() const noexcept {
		return cos_umbra;
	}

	[[nodiscard]]
	f32 getUmbraAngle() const noexcept {
		return std::acos(cos_umbra);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Penumbra
	//----------------------------------------------------------------------------------
	void setPenumbraCosAngle(f32 cos_angle) noexcept {
		cos_penumbra = std::max(std::min(cos_angle, cos_umbra - 0.001f), 0.001f);
		updateBoundingVolumes();
	}

	void setPenumbraAngle(f32 angle) noexcept {
		setPenumbraCosAngle(std::cos(angle));
	}

	[[nodiscard]]
	f32 getPenumbra() const noexcept {
		return cos_penumbra;
	}

	[[nodiscard]]
	f32 getPenumbraAngle() const noexcept {
		return std::acos(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Umbra/Penumbra
	//----------------------------------------------------------------------------------
	void setCutoffAngles(f32 cos_umbra, f32 cos_penumbra) noexcept {
		setUmbraAngle(cos_umbra);
		setPenumbraAngle(cos_penumbra);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Range
	//----------------------------------------------------------------------------------
	void setRange(f32 range) noexcept {
		this->range = std::max(0.01f, range);
		updateBoundingVolumes();
	}

	[[nodiscard]]
	f32 getRange() const noexcept {
		return range;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------
	void setShadows(bool state) noexcept {
		shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const noexcept {
		return shadows;
	}


	// Get the AABB of this light
	[[nodiscard]]
	const AABB& getAABB() const noexcept {
		return aabb;
	}

	// Get the bounding sphere of this light
	[[nodiscard]]
	const BoundingSphere& getBoundingSphere() const noexcept {
		return sphere;
	}


	// Get the light-to-projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getLightToProjectionMatrix() const {
		const f32 fov = std::acos(cos_penumbra) * 2.0f;
		return XMMatrixPerspectiveFovLH(fov, 1.0f, near_plane, range);
	}


private:
	void updateBoundingVolumes() noexcept {
		const f32 a   = 1.0f / (cos_penumbra * cos_penumbra);
		const f32 rxy = range * std::sqrt(a - 1.0f);
		const f32 rz  = range * 0.5f;
		const f32 r   = std::sqrt((rxy * rxy) + (rz * rz));

		aabb   = AABB{ f32_3{-rxy, -rxy, 0.0f}, f32_3{rxy, rxy, range} };
		sphere = BoundingSphere{ f32_3{0.0f, 0.0f, rz}, r };
	}


private:
	// Lighting parameters
	f32_3 base_color;
	f32   intensity;
	f32_3 attenuation;
	f32   cos_umbra;
	f32   cos_penumbra;

	// Near clipping plane and range (far plane)
	f32 near_plane;
	f32 range;

	// Flag that decides if the light should cast shadows
	bool shadows;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};

#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


class PointLight final : public Component<PointLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	PointLight() noexcept
		: base_color(1.0f, 1.0f, 1.0f)
	    , intensity(1.0f)
		, attenuation(0.0f, 0.0f, 1.0f)
		, near_plane(0.1f)
		, range(1.0f)
		, shadows(false) {
	}

	PointLight(const PointLight& light) = delete;
	PointLight(PointLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~PointLight() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	PointLight& operator=(const PointLight& light) = delete;
	PointLight& operator=(PointLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Base Color
	//----------------------------------------------------------------------------------
	void setBaseColor(vec3_f32 color) noexcept {
		base_color = std::move(color);
	}

	[[nodiscard]]
	vec3_f32& getBaseColor() noexcept {
		return base_color;
	}

	[[nodiscard]]
	const vec3_f32& getBaseColor() const noexcept {
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
	// Member Functions - Attenuation
	//----------------------------------------------------------------------------------
	void setAttenuation(vec3_f32 atten) noexcept {
		attenuation = std::move(atten);
	}

	[[nodiscard]]
	vec3_f32& getAttenuation() noexcept {
		return attenuation;
	}

	[[nodiscard]]
	const vec3_f32& getAttenuation() const noexcept {
		return attenuation;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------
	void setShadows(bool state) noexcept {
		this->shadows = state;
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
	XMMATRIX XM_CALLCONV getLightToProjectionMatrix() const noexcept {

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
	void updateBoundingVolumes() noexcept {
		aabb   = AABB{ vec3_f32{-range, -range, -range}, vec3_f32{range, range, range} };
		sphere = BoundingSphere{ vec3_f32{0.0f, 0.0f, 0.0f}, range };
	}


private:
	// Lighting parameters
	vec3_f32 base_color;
	f32      intensity;
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

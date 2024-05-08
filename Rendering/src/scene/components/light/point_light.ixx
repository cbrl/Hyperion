module;

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

#include "directxmath/directxmath_wrapper.h"

export module rendering:components.light.point_light;

import ecs;
import math.geometry;


export class PointLight final : public ecs::Component {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	PointLight() noexcept = default;
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
		aabb   = AABB{ f32_3{-range, -range, -range}, f32_3{range, range, range} };
		sphere = BoundingSphere{ f32_3{0.0f, 0.0f, 0.0f}, range };
	}


private:
	// Lighting parameters
	f32_3 base_color  = {1.0f, 1.0f, 1.0f};
	f32   intensity   = 1.0f;
	f32_3 attenuation = {0.0f, 0.0f, 1.0f};

	// Near clipping plane and range (far plane)
	f32 near_plane = 0.1f;
	f32 range      = 1.0f;

	// Flag that decides if the light should cast shadows
	bool shadows = false;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;
};
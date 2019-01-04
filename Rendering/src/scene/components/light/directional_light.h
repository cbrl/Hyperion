#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


class DirectionalLight final : public Component<DirectionalLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DirectionalLight() noexcept
		: base_color(1.0f, 1.0f, 1.0f)
	    , intensity(1.0f)
		, start(0.1f)
		, range(1.0f)
		, proj_size(1.0f, 1.0f)
		, shadows(false) {
	}

	DirectionalLight(const DirectionalLight& light) = delete;
	DirectionalLight(DirectionalLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~DirectionalLight() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	DirectionalLight& operator=(const DirectionalLight& light) = delete;
	DirectionalLight& operator=(DirectionalLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Base Color
	//----------------------------------------------------------------------------------
	void setBaseColor(const vec3_f32& color) noexcept {
		base_color = color;
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

	[[nodiscard]]
	f32 getIntensity() const noexcept {
		return intensity;
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
	// Member Functions - Size
	//----------------------------------------------------------------------------------
	void setSize(const vec2_f32& size) noexcept {
		proj_size = size;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	const vec2_f32& getSize() const noexcept {
		return proj_size;
	}


	// Get the AABB of this light
	[[nodiscard]]
	const AABB& getAABB() const noexcept {
		return aabb;
	}

	// Get the light-to-projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getLightToProjectionMatrix() const {
		return XMMatrixOrthographicLH(proj_size[0], proj_size[1], start, range);
	}


private:
	void updateBoundingVolumes() noexcept {
		aabb = AABB{ vec3_f32{-0.5f * proj_size[0], -0.5f * proj_size[1], 0.0f},
					 vec3_f32{ 0.5f * proj_size[0],  0.5f * proj_size[1], range} };
	}


private:
	// Lighting parameters
	vec3_f32 base_color;
	f32 intensity;

	// Clipping planes
	f32 start;
	f32 range;

	// Projection matrix size
	vec2_f32 proj_size;

	// Bounding volume
	AABB aabb;

	// Shadow boolean
	bool shadows;
};

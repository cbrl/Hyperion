#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


namespace render {

class DirectionalLight final : public ecs::Component<DirectionalLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DirectionalLight() noexcept = default;
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
	void setBaseColor(const f32_3& color) noexcept {
		base_color = color;
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
	void setSize(const f32_2& size) noexcept {
		proj_size = size;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	const f32_2& getSize() const noexcept {
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
		aabb = AABB{ f32_3{-0.5f * proj_size[0], -0.5f * proj_size[1], 0.0f},
					 f32_3{ 0.5f * proj_size[0],  0.5f * proj_size[1], range} };
	}


private:
	// Lighting parameters
	f32_3 base_color = {1.0f, 1.0f, 1.0f};
	f32   intensity  = 1.0f;

	// Clipping planes
	f32 start = 0.1f;
	f32 range = 1.0f;

	// Projection matrix size
	f32_2 proj_size = {1.0f, 1.0f};

	// Bounding volume
	AABB aabb;

	// Shadow boolean
	bool shadows = false;
};

} //namespace render
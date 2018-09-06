#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


class DirectionalLight final : public Component<DirectionalLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DirectionalLight() noexcept
		: diffuse_color(1.0f, 1.0f, 1.0f, 0.0f)
		, specular(1.0f, 1.0f, 1.0f, 1.0f)
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
	// Member Functions - Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const vec4_f32& color) {
		this->diffuse_color = color;
	}

	[[nodiscard]]
	const vec4_f32& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const vec4_f32& spec) {
		this->specular = spec;
	}

	[[nodiscard]]
	const vec4_f32& getSpecular() const {
		return specular;
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


	//----------------------------------------------------------------------------------
	// Member Functions - Range
	//----------------------------------------------------------------------------------
	void setRange(f32 range) {
		this->range = std::max(0.01f, range);
		updateBoundingVolumes();
	}

	[[nodiscard]]
	f32 getRange() const {
		return range;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Size
	//----------------------------------------------------------------------------------
	void setSize(const vec2_f32& size) {
		proj_size = size;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	const vec2_f32& getSize() const {
		return proj_size;
	}


	// Get the AABB of this light
	[[nodiscard]]
	const AABB& getAABB() const {
		return aabb;
	}

	// Get the light-to-projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getLightToProjectionMatrix() const {
		return XMMatrixOrthographicLH(proj_size.x, proj_size.y, start, range);
	}


private:
	void updateBoundingVolumes() {
		aabb = AABB{ vec3_f32{-0.5f * proj_size.x, -0.5f * proj_size.y, 0.0f},
					 vec3_f32{ 0.5f * proj_size.x,  0.5f * proj_size.y, range} };
	}


private:
	// Lighting parameters
	vec4_f32 diffuse_color;
	vec4_f32 specular;

	// Clipping planes
	f32      start;
	f32      range;

	// Projection matrix size
	vec2_f32 proj_size;

	// Bounding volume
	AABB     aabb;

	// Shadow boolean
	bool shadows;
};

#pragma	once

#include "component/component.h"
#include "geometry/bounding_volume/bounding_volume.h"


class DirectionalLight final : public Component<DirectionalLight> {
public:
	DirectionalLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 0.0f)
		, diffuse_color(1.0f, 1.0f, 1.0f, 0.0f)
		, specular(1.0f, 1.0f, 1.0f, 1.0f)
		, start(0.1f)
		, range(1.0f)
		, proj_size(1.0f, 1.0f)
		, shadows(false) {
	}

	~DirectionalLight() = default;

	//----------------------------------------------------------------------------------
	// Ambient Color
	//----------------------------------------------------------------------------------
	void setAmbientColor(const f32_4& color) {
		this->ambient_color = color;
	}

	[[nodiscard]]
	const f32_4& getAmbientColor() const {
		return ambient_color;
	}


	//----------------------------------------------------------------------------------
	// Diffuse Color
	//----------------------------------------------------------------------------------
	void setDiffuseColor(const f32_4& color) {
		this->diffuse_color = color;
	}

	[[nodiscard]]
	const f32_4& getDiffuseColor() const {
		return diffuse_color;
	}


	//----------------------------------------------------------------------------------
	// Specular Color/Power
	//----------------------------------------------------------------------------------
	void setSpecular(const f32_4& spec) {
		this->specular = spec;
	}

	[[nodiscard]]
	const f32_4& getSpecular() const {
		return specular;
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
	// Size
	//----------------------------------------------------------------------------------
	void setSize(const f32_2& size) {
		proj_size = size;
		updateBoundingVolumes();
	}

	[[nodiscard]]
	const f32_2& getSize() const {
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
		aabb = AABB{ f32_3{-0.5f * proj_size.x, -0.5f * proj_size.y, 0.0f},
					 f32_3{ 0.5f * proj_size.x,  0.5f * proj_size.y, range} };
	}


private:
	f32_4 ambient_color;
	f32_4 diffuse_color;
	f32_4 specular;
	f32   start;
	f32   range;
	f32_2 proj_size;
	AABB  aabb;

	bool shadows;
};

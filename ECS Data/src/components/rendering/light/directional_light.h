#pragma	once

#include "component/component.h"


class DirectionalLight final : public Component<DirectionalLight> {
public:
	DirectionalLight()
		: shadows(false) {
	}

	~DirectionalLight() = default;

	void setAmbientColor(const float4& color) {
		this->ambient_color = color;
	}

	void setDiffuseColor(const float4& color) {
		this->diffuse_color = color;
	}

	void setSpecular(const float4& spec) {
		this->specular = spec;
	}

	void setShadows(bool state) {
		shadows = state;
	}


	const float4& getAmbientColor() const {
		return ambient_color;
	}

	const float4& getDiffuseColor() const {
		return diffuse_color;
	}

	const float4& getSpecular() const {
		return specular;
	}

	bool castsShadows() const {
		return shadows;
	}


private:
	float4 ambient_color;
	float4 diffuse_color;
	float4 specular;

	bool shadows;
};

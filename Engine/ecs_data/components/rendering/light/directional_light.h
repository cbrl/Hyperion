#pragma	once

#include "ecs\component\component.h"


class DirectionalLight final : public Component<DirectionalLight> {
	public:
		DirectionalLight() : shadows(false) {}
		~DirectionalLight() = default;

		void SetAmbientColor(const float4& color) {
			this->ambient_color = color;
		}

		void SetDiffuseColor(const float4& color) {
			this->diffuse_color = color;
		}

		void SetSpecular(const float4& spec) {
			this->specular = spec;
		}

		void SetShadows(bool state) {
			shadows = state;
		}


		const float4& GetAmbientColor() const {
			return ambient_color;
		}

		const float4& GetDiffuseColor() const {
			return diffuse_color;
		}

		const float4& GetSpecular() const {
			return specular;
		}

		bool CastsShadows() const {
			return shadows;
		}


	private:
		float4 ambient_color;
		float4 diffuse_color;
		float4 specular;

		bool shadows;
};
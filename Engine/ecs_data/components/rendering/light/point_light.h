#pragma	once

#include "ecs\component\component.h"


class PointLight final : public Component<PointLight> {
	public:
		PointLight() : shadows(false) {}
		~PointLight() = default;

		void SetAmbientColor(const float4& color) {
			this->ambient_color = color;
		}

		void SetDiffuseColor(const float4& color) {
			this->diffuse_color = color;
		}

		void SetSpecular(const float4& spec) {
			this->specular = spec;
		}

		void SetRange(const float range) {
			this->range = range;
		}

		void SetAttenuation(const float3& attenuation) {
			this->attenuation = attenuation;
		}

		void SetShadows(bool state) {
			this->shadows = state;
		}

		bool CastsShadows() const {
			return shadows;
		}


	private:
		float4 ambient_color;
		float4 diffuse_color;
		float4 specular;
		float3 attenuation;
		float  range;

		bool shadows;
};
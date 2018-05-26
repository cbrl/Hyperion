#pragma	once

#include "component\component.h"
#include "geometry\bounding_volume\bounding_volume.h"


class PointLight final : public Component<PointLight> {
	public:
		PointLight() : near_plane(0.1f), shadows(false) {}
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
			UpdateBoundingVolumes();
		}

		void SetAttenuation(const float3& attenuation) {
			this->attenuation = attenuation;
		}

		void SetShadows(bool state) {
			this->shadows = state;
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

		const float3& GetAttenuation() const {
			return attenuation;
		}

		const float GetRange() const {
			return range;
		}

		bool CastsShadows() const {
			return shadows;
		}

		const AABB& GetAABB() const {
			return aabb;
		}

		const BoundingSphere& GetBoundingSphere() const {
			return sphere;
		}

		const XMMATRIX XM_CALLCONV GetLightToProjectionMatrix() const {

			const float m22 = range / (range - near_plane);
			const float m32 = -near_plane * m22;

			return XMMATRIX{ 1.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f, 0.0f,
							 0.0f, 0.0f,  m22, 1.0f,
			                 0.0f, 0.0f,  m32, 0.0f };
		}


	private:
		void UpdateBoundingVolumes() {
			aabb   = AABB(float3(-range, -range, -range), float3(range, range, range));
			sphere = BoundingSphere(float3(0.0f, 0.0f, 0.0f), range);
		}


	private:
		// Lighting parameters
		float4 ambient_color;
		float4 diffuse_color;
		float4 specular;
		float3 attenuation;

		// Near clipping plane and range (far plane)
		float near_plane;
		float range;

		// Flag that decides if the light should cast shadows
		bool shadows;

		// Bounding volumes
		AABB           aabb;
		BoundingSphere sphere;
};
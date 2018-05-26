#pragma	once

#include "component\component.h"


class SpotLight final : public Component<SpotLight> {
	public:
		SpotLight() : near_plane(0.1f), shadows(false) {}
		~SpotLight() = default;

		void SetAmbientColor(const float4& color) {
			ambient_color = color;
		}

		void SetDiffuseColor(const float4& color) {
			diffuse_color = color;
		}

		void SetSpecular(const float4& spec) {
			specular = spec;
		}

		void SetAttenuation(const float3& atten) {
			attenuation = atten;
		}

		void SetUmbraCosAngle(float cos_angle) {
			cos_umbra = std::max(cos_angle, 0.001f);
			UpdateBoundingVolumes();
		}

		void SetUmbraAngle(float angle) {
			SetUmbraCosAngle(std::cos(angle));
		}

		void SetPenumbraCosAngle(float cos_angle) {
			cos_penumbra = std::max(cos_angle, cos_umbra + 0.001f);
		}

		void SetPenumbraAngle(float angle) {
			SetPenumbraCosAngle(std::cos(angle));
		}

		void SetCutoffAngles(float cos_umbra, float cos_penumbra) {
			SetUmbraAngle(cos_umbra);
			SetPenumbraAngle(cos_penumbra);
		}

		void SetRange(float range) {
			this->range = range;
			UpdateBoundingVolumes();
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

		const float3& GetAttenuation() const {
			return attenuation;
		}

		float GetUmbra() const {
			return cos_umbra;
		}

		float GetUmbraAngle() const {
			return std::acos(cos_umbra);
		}

		float GetPenumbra() const {
			return cos_penumbra;
		}

		float GetPenumbraAngle() const {
			return std::acos(cos_penumbra);
		}

		float GetRange() const {
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
			const float fov = std::acos(cos_umbra) * 2.0f;
			return XMMatrixPerspectiveFovLH(fov, 1.0f, near_plane, range);
		}


	private:
		void UpdateBoundingVolumes() {
			const float a   = 1.0f / (cos_umbra * cos_umbra);
			const float rxy = range * std::sqrt(a - 1.0f);
			const float rz  = range * 0.5f;
			const float r   = std::sqrt((rxy * rxy) + (rz * rz));
			
			aabb   = AABB(float3(-rxy, -rxy, 0.0f), float3(rxy, rxy, range));
			sphere = BoundingSphere(float3(0.0f, 0.0f, rz), r);
		}


	private:
		// Lighting parameters
		float4 ambient_color;
		float4 diffuse_color;
		float4 specular;
		float3 attenuation;
		float  cos_umbra;
		float  cos_penumbra;

		// Near clipping plane and range (far plane)
		float near_plane;
		float range;

		// Flag that decides if the light should cast shadows
		bool shadows;

		// Bounding volumes
		AABB           aabb;
		BoundingSphere sphere;
};
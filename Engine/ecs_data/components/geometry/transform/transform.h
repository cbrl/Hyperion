#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "ecs\component\component.h"


class Transform final : public Component<Transform> {
	friend class TransformSystem;

	public:
		Transform();
		~Transform() = default;

		bool Updated() const { return updated; }

		void Move(const float3& move);
		void SetPosition(const float3& position);
		void XM_CALLCONV Move(FXMVECTOR move);
		void XM_CALLCONV SetPosition(FXMVECTOR position);
		
		void Rotate(const float3& rotate);
		void SetRotation(const float3& rotation);
		void XM_CALLCONV Rotate(FXMVECTOR rotate);
		void XM_CALLCONV SetRotation(FXMVECTOR rotation);

		void Scale(const float3& scale);
		void SetScale(const float3& scale);
		void XM_CALLCONV Scale(FXMVECTOR scale);
		void XM_CALLCONV SetScale(FXMVECTOR scale);

		const XMMATRIX GetWorld()    const { return world; }
		const XMVECTOR GetPosition() const { return translation; }
		const XMVECTOR GetRotation() const { return rotation; }
		const XMVECTOR GetScale()    const { return scale; }

		const XMVECTOR GetXAxis() const { return XMVector3Normalize(world.r[0]); }
		const XMVECTOR GetYAxis() const { return XMVector3Normalize(world.r[1]); }
		const XMVECTOR GetZAxis() const { return XMVector3Normalize(world.r[2]); }

		const XMMATRIX GetPositionMatrix() const { return XMMatrixTranslationFromVector(translation); }
		const XMMATRIX GetRotationMatrix() const { return XMMatrixRotationRollPitchYawFromVector(rotation); }
		const XMMATRIX GetScaleMatrix()    const { return XMMatrixScalingFromVector(scale); }


	private:
		// The parent of the entity this transform is assigned to (optional)
		Handle64 parent;

		// The object-to-world matrix
		XMMATRIX world;
		
		// Translation, rotation, and scale vectors
		XMVECTOR translation;
		XMVECTOR rotation;
		XMVECTOR scale;

		// Flag that decides if the object-to-world matrix requires an update
		bool needs_update;

		// Has the transform been updated in the past fame?
		bool updated;
};
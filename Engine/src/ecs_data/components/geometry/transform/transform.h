#pragma once

#include <d3d11.h>
#include "datatypes\datatypes.h"
#include "component\component.h"


class Transform final : public Component<Transform> {
	friend class TransformSystem;

	public:
		Transform();
		~Transform() = default;

		bool Updated() const { return updated; }


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		void SetParent(const Handle64 handle) { parent = handle; }

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


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		// Get the entity whose transform is a parent of this transform
		const Handle64 GetParent() const { return parent; }

		// Position, Rotation, and Scale (relative to parent, if any)
		const XMVECTOR XM_CALLCONV GetPosition() const { return translation; }
		const XMVECTOR XM_CALLCONV GetRotation() const { return rotation; }
		const XMVECTOR XM_CALLCONV GetScale()    const { return scale; }

		// World space data
		const XMVECTOR XM_CALLCONV GetWorldAxisX()  const { return XMVector3Normalize(world.r[0]); }
		const XMVECTOR XM_CALLCONV GetWorldAxisY()  const { return XMVector3Normalize(world.r[1]); }
		const XMVECTOR XM_CALLCONV GetWorldAxisZ()  const { return XMVector3Normalize(world.r[2]); }
		const XMVECTOR XM_CALLCONV GetWorldOrigin() const { return world.r[3]; }

		// Object space data
		const XMVECTOR XM_CALLCONV GetObjectAxisX()  const { return XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); }
		const XMVECTOR XM_CALLCONV GetObjectAxisY()  const { return XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); }
		const XMVECTOR XM_CALLCONV GetObjectAxisZ()  const { return XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); }
		const XMVECTOR XM_CALLCONV GetObjectOrigin() const { return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }

		// Matrices
		const XMMATRIX XM_CALLCONV GetObjectToWorldMatrix()          const { return world; }
		const XMMATRIX XM_CALLCONV GetWorldToObjectMatrix()          const { return XMMatrixInverse(NULL, world); }
		const XMMATRIX XM_CALLCONV GetObjectToParentPositionMatrix() const { return XMMatrixTranslationFromVector(translation); }
		const XMMATRIX XM_CALLCONV GetObjectToParentRotationMatrix() const { return XMMatrixRotationRollPitchYawFromVector(rotation); }
		const XMMATRIX XM_CALLCONV GetObjectToParentScaleMatrix()    const { return XMMatrixScalingFromVector(scale); }


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
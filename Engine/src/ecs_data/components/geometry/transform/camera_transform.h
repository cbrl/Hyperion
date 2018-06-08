#pragma once

#include "component\component.h"


struct CameraTransform final : public Component<CameraTransform> {
	friend class TransformSystem;

	public:
		CameraTransform();
		~CameraTransform() = default;


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const handle64 GetParent() const { return parent; }

		// Relative position and rotation
		const XMVECTOR XM_CALLCONV GetPosition() const { return position; }
		const float2 GetPitchYaw() const { return float2(pitch, yaw); }

		// World space data
		const XMVECTOR XM_CALLCONV GetWorldAxisX()  const { return right; }
		const XMVECTOR XM_CALLCONV GetWorldAxisY()  const { return up; }
		const XMVECTOR XM_CALLCONV GetWorldAxisZ()  const { return forward; }
		const XMVECTOR XM_CALLCONV GetWorldOrigin() const { return world.r[3]; }

		// Object space data
		const XMVECTOR XM_CALLCONV GetObjectAxisX()  const { return XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); }
		const XMVECTOR XM_CALLCONV GetObjectAxisY()  const { return XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); }
		const XMVECTOR XM_CALLCONV GetObjectAxisZ()  const { return XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); }
		const XMVECTOR XM_CALLCONV GetObjectOrigin() const { return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }

		// Matrices
		const XMMATRIX XM_CALLCONV GetObjectToWorldMatrix()          const { return world; }
		const XMMATRIX XM_CALLCONV GetWorldToObjectMatrix()          const { return XMMatrixInverse(NULL, world); }
		const XMMATRIX XM_CALLCONV GetObjectToParentPositionMatrix() const { return XMMatrixTranslationFromVector(position); }


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		void SetParent(const handle64 handle) { parent = handle; }

		void SetRotation(const float3& rotation);
		void XM_CALLCONV SetRotation(FXMVECTOR rotation);

		void Rotate(const float3& units);

		void SetPosition(const float3& position);
		void XM_CALLCONV SetPosition(FXMVECTOR position);

		void Move(const float3& units);
		void XM_CALLCONV Move(FXMVECTOR units);


	private:
		void RotateX(const float units);
		void RotateY(const float units);
		void RotateZ(const float units);


	private:
		handle64 parent;

		XMMATRIX world;

		XMVECTOR position;

		XMVECTOR right;
		XMVECTOR up;
		XMVECTOR forward;

		float pitch;
		float max_pitch;
		float yaw;

		bool needs_update;
		bool updated;
};
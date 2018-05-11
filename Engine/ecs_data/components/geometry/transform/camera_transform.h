#pragma once

#include "ecs\component\component.h"


struct CameraTransform final : public Component<CameraTransform> {
	friend class TransformSystem;

	public:
		CameraTransform();
		~CameraTransform() = default;


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const Handle64 GetParent() const { return parent; }

		const XMVECTOR GetPosition()       const { return position; }
		const XMMATRIX GetPositionMatrix() const { return XMMatrixTranslationFromVector(position); }

		const float2   GetPitchYaw() const { return float2(pitch, yaw); }

		const XMVECTOR GetAxisX() const { return right; }
		const XMVECTOR GetAxisY() const { return up; }
		const XMVECTOR GetAxisZ() const { return forward; }

		const XMMATRIX GetWorld() const { return world; }


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		void SetParent(const Handle64 handle) { parent = handle; }

		void SetRotation(const float3& rotation);
		void XM_CALLCONV SetRotation(FXMVECTOR rotation);

		void Rotate(const float3& units);

		void Move(const float3& units);
		void XM_CALLCONV Move(FXMVECTOR units);


	private:
		void RotateX(const float units);
		void RotateY(const float units);
		void RotateZ(const float units);


	private:
		Handle64 parent;

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
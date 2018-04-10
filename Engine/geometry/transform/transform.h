#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"


class Transform final {
	public:
		Transform();
		~Transform() = default;

		void Update();

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

		const XMMATRIX& GetWorld()    const { return world; }
		const XMVECTOR& GetPosition() const { return translation; }
		const XMVECTOR& GetRotation() const { return rotation; }
		const XMVECTOR& GetScale()    const { return scale; }


	private:
		XMMATRIX world;
		
		XMVECTOR translation;
		XMVECTOR rotation;
		XMVECTOR scale;

		bool needs_update;
};
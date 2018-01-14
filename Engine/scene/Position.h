#pragma once

#include <d3d11.h>
#include "input\Input.h"

using namespace DirectX;

class Position {
	public:
		Position() = default;
		~Position() = default;

		static void UpdatePosition(Input& input, float deltaTime);

		static XMFLOAT3 m_TurnSpeed;
		static XMFLOAT3 m_Rotation;
};


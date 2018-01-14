#include "stdafx.h"
#include "Position.h"


XMFLOAT3 Position::m_Rotation(0.0f, 0.0f, 0.0f);
XMFLOAT3 Position::m_TurnSpeed(0.0f, 0.0f, 0.0f);


void Position::UpdatePosition(Input& input, float deltaTime) {
	// Update Y turn speed
	if (input.IsKeyPressed(DIK_A)) {
		m_TurnSpeed.y -= deltaTime * 0.015f;

		if (m_TurnSpeed.y < -(deltaTime * 0.15f)) {
			m_TurnSpeed.y = -(deltaTime * 0.15f);
		}
	}
	else if (input.IsKeyPressed(DIK_D)) {
		m_TurnSpeed.y += deltaTime * 0.015f;

		if (m_TurnSpeed.y > deltaTime * 0.15f) {
			m_TurnSpeed.y = deltaTime * 0.15f;
		}
	}
	else {
		if (m_TurnSpeed.y < 0.0f) {
			m_TurnSpeed.y += deltaTime * 0.005f;
		}
		else if (m_TurnSpeed.y > 0.0f) {
			m_TurnSpeed.y -= deltaTime * 0.005f;
		}
	}

	// Update Y rotation
	m_Rotation.y += m_TurnSpeed.y;

	if (m_Rotation.y > 360.0f) {
		m_Rotation.y = -360.0f;
	}
	else if (m_Rotation.y < -360.0f) {
		m_Rotation.y = 360.0f;
	}



	// Update X turn speed
	if (input.IsKeyPressed(DIK_W)) {
		m_TurnSpeed.x -= deltaTime * 0.015f;

		if (m_TurnSpeed.x < -(deltaTime * 0.15f)) {
			m_TurnSpeed.x = -(deltaTime * 0.15f);
		}
	}
	else if (input.IsKeyPressed(DIK_S)) {
		m_TurnSpeed.x += deltaTime * 0.015f;

		if (m_TurnSpeed.x > deltaTime * 0.15f) {
			m_TurnSpeed.x = deltaTime * 0.15f;
		}
	}
	else {
		if (m_TurnSpeed.x < 0.0f) {
			m_TurnSpeed.x += deltaTime * 0.005f;
		}
		else if (m_TurnSpeed.x > 0.0f) {
			m_TurnSpeed.x -= deltaTime * 0.005f;
		}
	}

	// Update X rotation
	m_Rotation.x += m_TurnSpeed.x;

	if (m_Rotation.x > 360.0f) {
		m_Rotation.x = -360.0f;
	}
	else if (m_Rotation.x < -360.0f) {
		m_Rotation.x = 360.0f;
	}
}
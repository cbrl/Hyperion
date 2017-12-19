#pragma once

using namespace DirectX;

class Camera {
	public:
		Camera();
		~Camera();

		void SetPosition(XMFLOAT3 position);
		void SetRotation(XMFLOAT3 rotation);
		void Render();

		XMFLOAT3 GetPosition();
		XMFLOAT3 GetRotation();
		XMMATRIX GetViewMatrix();

	private:
		XMFLOAT3 m_Position;
		XMFLOAT3 m_Rotation;
		XMMATRIX m_ViewMatrix;
};


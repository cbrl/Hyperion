#pragma once

#include "EngineUtil.h"
#include "ShaderBase.h"
#include "Light.h"
#include <d3d11.h>
#include <DirectXMath.h>

using std::unique_ptr;

class LightShader : public ShaderBase {
	public:
		LightShader();
		~LightShader();

		void CreatePolygonLayout();

		bool InitBuffers(const ComPtr<ID3D11Device>& device);

		void SetShader(const ComPtr<ID3D11DeviceContext>& deviceContext);
		void SetCBuffers(const ComPtr<ID3D11DeviceContext>& deviceContext);
		void SetParameters(const ComPtr<ID3D11DeviceContext>& deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, ComPtr<ID3D11ShaderResourceView> texture, Light & light);
		void Render(const ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount);

		//bool Render(const ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		//			XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, ComPtr<ID3D11ShaderResourceView> texture, XMFLOAT3 lightDirection,
		//			XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower);

		//bool SetShaderParameters(const ComPtr<ID3D11DeviceContext>& deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		//						 XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, ComPtr<ID3D11ShaderResourceView> texture,
		//						 XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower);

		//void RenderShader(const ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount);


	private:
		struct LightBuffer {
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT3 lightDirection;
			float specularPower;
			XMFLOAT4 specularColor;
		};

		struct CameraBuffer {
			XMFLOAT3 cameraPosition;
			float padding;
		};


	private:
		D3D11_BUFFER_DESC    m_CameraBufferDesc;
		D3D11_BUFFER_DESC    m_LightBufferDesc;

		ComPtr<ID3D11Buffer> m_CameraBuffer;
		ComPtr<ID3D11Buffer> m_LightBuffer;
};


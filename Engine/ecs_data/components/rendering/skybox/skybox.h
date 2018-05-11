#pragma once

#include <d3d11.h>
#include "ecs\component\component.h"
#include "resource\texture\texture.h"
#include "resource\resource_mgr.h"


class SkyBox final : public Component<SkyBox> {
	public:
		SkyBox() = default;
		~SkyBox() = default;

		SkyBox(ID3D11Device& device,
			   ResourceMgr& resource_mgr,
			   const wstring& filename) {
			Init(device, resource_mgr.GetOrCreate<Texture>(filename));
		}

		SkyBox(ID3D11Device& device,
			   shared_ptr<Texture> texture) {
			Init(device, texture);
		}

		void Init(ID3D11Device& device,
				  shared_ptr<Texture> tex);

		// Bind the skybox's vertex and index buffers
		void Bind(ID3D11DeviceContext& device_context) const {
			u32 offset = 0;

			Pipeline::IA::BindVertexBuffers(device_context, 0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
			Pipeline::IA::BindIndexBuffer(device_context, index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			Pipeline::IA::BindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		// Draw the skybox
		void Draw(ID3D11DeviceContext& device_context) const {
			device_context.DrawIndexed(index_count, 0, 0);
		}

		ID3D11ShaderResourceView* GetSRV() const {
			return texture->Get();
		}

		shared_ptr<Texture> GetTexture() const {
			return texture;
		}

		void SetTexture(shared_ptr<Texture> tex) {
			texture = std::move(tex);
		}


	private:
		ComPtr<ID3D11Buffer> vertex_buffer;
		ComPtr<ID3D11Buffer> index_buffer;
		u32 index_count;
		u32 stride;

		shared_ptr<Texture> texture;
};
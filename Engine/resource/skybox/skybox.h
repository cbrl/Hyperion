#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "resource\texture\texture.h"
#include "resource\resource_mgr.h"
#include "geometry\shapes\shapes.h"


class SkyBox {
	public:
		SkyBox() = default;

		SkyBox(ID3D11Device* device, ResourceMgr& resource_mgr, const wstring& filename) {
			Init(device, resource_mgr, filename);
		}

		~SkyBox() = default;

		void Init(ID3D11Device* device, ResourceMgr& resource_mgr, const wstring& filename) {

			// Create the texture
			texture = resource_mgr.Create<Texture>(filename);

			// Set the vertex stride
			stride = sizeof(VertexPosition);


			// Create a sphere
			vector<VertexPosition> vertices;
			vector<u32> indices;

			ComputeSphere(vertices, indices, 5000.0f, 16, true, true);


			// Create the vertex buffer
			D3D11_BUFFER_DESC vb_desc;
			vb_desc.Usage               = D3D11_USAGE_IMMUTABLE;
			vb_desc.ByteWidth           = static_cast<u32>(sizeof(float3) * vertices.size());
			vb_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
			vb_desc.CPUAccessFlags      = 0;
			vb_desc.MiscFlags           = 0;
			vb_desc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vb_data;
			vb_data.pSysMem = &vertices[0];

			device->CreateBuffer(&vb_desc, &vb_data, vertex_buffer.GetAddressOf());


			// Create the index buffer
			index_count = static_cast<u32>(indices.size());

			D3D11_BUFFER_DESC ib_desc;
			ib_desc.Usage               = D3D11_USAGE_IMMUTABLE;
			ib_desc.ByteWidth           = sizeof(u32) * index_count;
			ib_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
			ib_desc.CPUAccessFlags      = 0;
			ib_desc.StructureByteStride = 0;
			ib_desc.MiscFlags           = 0;

			D3D11_SUBRESOURCE_DATA ib_data;
			ib_data.pSysMem = &indices[0];

			device->CreateBuffer(&ib_desc, &ib_data, index_buffer.GetAddressOf());
		}


		void Bind(ID3D11DeviceContext* device_context) {
			u32 offset = 0;

			Pipeline::IA::BindVertexBuffers(device_context, 0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

			Pipeline::IA::BindIndexBuffer(device_context, index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			Pipeline::IA::BindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void Draw(ID3D11DeviceContext* device_context) {
			device_context->DrawIndexed(index_count, 0, 0);
		}


		shared_ptr<Texture> GetTexture() const {
			return texture;
		}


	private:
		ComPtr<ID3D11Buffer> vertex_buffer;
		ComPtr<ID3D11Buffer> index_buffer;
		u32 index_count;
		u32 stride;

		shared_ptr<Texture> texture;
};
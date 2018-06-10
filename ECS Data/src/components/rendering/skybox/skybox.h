#pragma once

#include <d3d11.h>
#include "directx/vertex_types.h"
#include "component/component.h"
#include "resource/texture/texture.h"
#include "resource/resource_mgr.h"
#include "geometry/shapes/shapes.h"


class SkyBox final : public Component<SkyBox> {
public:
	SkyBox() = default;
	~SkyBox() = default;

	SkyBox(ID3D11Device& device,
	       ResourceMgr& resource_mgr,
	       const wstring& filename) {
		init(device, resource_mgr.getOrCreate<Texture>(filename));
	}

	SkyBox(ID3D11Device& device,
	       shared_ptr<Texture> texture) {
		init(device, texture);
	}

	void init(ID3D11Device& device,
	          shared_ptr<Texture> tex);

	// Bind the skybox's vertex and index buffers
	void bind(ID3D11DeviceContext& device_context) const {
		u32 offset = 0;

		Pipeline::IA::bindVertexBuffers(device_context, 0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
		Pipeline::IA::bindIndexBuffer(device_context, index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	// Draw the skybox
	void draw(ID3D11DeviceContext& device_context) const {
		device_context.DrawIndexed(index_count, 0, 0);
	}

	ID3D11ShaderResourceView* getSRV() const {
		return texture->get();
	}

	shared_ptr<Texture> getTexture() const {
		return texture;
	}

	void setTexture(shared_ptr<Texture> tex) {
		texture = std::move(tex);
	}


private:
	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;
	u32 index_count;
	u32 stride;

	shared_ptr<Texture> texture;
};

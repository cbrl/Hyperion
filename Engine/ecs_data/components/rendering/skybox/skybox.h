#pragma once

#include <d3d11.h>
#include "ecs\component\component.h"
#include "resource\texture\texture.h"
#include "resource\resource_mgr.h"
#include "geometry\shapes\shapes.h"


class SkyBox final : public Component<SkyBox> {
	public:
		SkyBox() = default;
		~SkyBox() = default;

		SkyBox(ID3D11Device* device,
			   ResourceMgr& resource_mgr,
			   const wstring& filename) {
			Init(device, resource_mgr, filename);
		}

		void Init(ID3D11Device* device,
				  ResourceMgr& resource_mgr,
				  const wstring& filename);

		// Bind the skybox's vertex and index buffers
		void Bind(ID3D11DeviceContext* device_context) const;

		// Draw the skybox
		void Draw(ID3D11DeviceContext* device_context) const {
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
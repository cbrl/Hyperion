#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "resource/resource.h"
#include "loader/texture_loader.h"


struct Texture final : public Resource<Texture> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Texture(ID3D11Device& device,
	        ID3D11DeviceContext& device_context,
	        const std::wstring& filename)
		: Resource(filename) {

		TextureLoader::LoadTexture(device, device_context, filename, texture.GetAddressOf());
	}

	Texture(ID3D11Device& device, u32 color)
		: Resource(std::to_wstring(color)) {

		TextureLoader::LoadTexture(device, color, texture.GetAddressOf());
	}

	Texture(const Texture& texture) = delete;
	Texture(Texture&& texture) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Texture() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Texture& operator=(const Texture& texture) = delete;
	Texture& operator=(Texture&& texture) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	ID3D11ShaderResourceView* get() const {
		return texture.Get();
	}

	// Bind the texture to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) const {

		StageT::bindSRV(device_context, slot, texture.Get());
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11ShaderResourceView> texture;
};

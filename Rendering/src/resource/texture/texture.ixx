module;

#include "datatypes/types.h"
#include "directx/d3d11.h"

export module rendering.texture;

export import rendering.resource;
import rendering.importer.texture_importer;


namespace render {

export class Texture final : public Resource<Texture> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Texture(ID3D11Device& device,
	        ID3D11DeviceContext& device_context,
	        const std::wstring& filename)
		: Resource(filename) {

		importer::ImportTexture(device, device_context, filename, texture_srv.GetAddressOf());
	}

	Texture(const std::wstring& guid,
	        ID3D11Device& device,
	        const D3D11_TEXTURE2D_DESC& desc,
	        const D3D11_SUBRESOURCE_DATA& init_data)
		: Resource(guid) {

		HRESULT hr;
		ComPtr<ID3D11Texture2D> texture;

		hr = device.CreateTexture2D(&desc,
									&init_data,
									texture.ReleaseAndGetAddressOf());
		ThrowIfFailed(hr, "Failed to create Texture2D");

		hr = device.CreateShaderResourceView(texture.Get(),
											 nullptr,
											 texture_srv.ReleaseAndGetAddressOf());
		ThrowIfFailed(hr, "Failed to create Texture SRV");
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
		return texture_srv.Get();
	}

	// Bind the texture to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) const {
		StageT::bindSRV(device_context, slot, texture_srv.Get());
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11ShaderResourceView> texture_srv;
};

} //namespace render

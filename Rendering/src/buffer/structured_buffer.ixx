module;

#include "datatypes/types.h"

#include "directx/directxtk.h"
#include "directx/d3d11.h"

export module rendering:structured_buffer;


export namespace render {

template<typename DataT>
class StructuredBuffer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	StructuredBuffer(ID3D11Device& device, u32 reserved_size)
		: current_size(0)
		, reserved_size(reserved_size) {

		createBuffer(device);
	}

	StructuredBuffer(const StructuredBuffer& buffer) = delete;
	StructuredBuffer(StructuredBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~StructuredBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	StructuredBuffer& operator=(const StructuredBuffer& buffer) = delete;
	StructuredBuffer& operator=(StructuredBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void updateData(ID3D11Device& device,
	                ID3D11DeviceContext& device_context,
	                const std::vector<DataT>& data) {
		// Set the number of elements used
		current_size = static_cast<u32>(data.size());

		if (data.size() == 0) return;

		// Recreate the buffer if the array of data being fed to it is larger than the buffer
		if (current_size > reserved_size) {
			reserved_size = current_size;
			createBuffer(device);
		}

		// Map the buffer and update the data
		D3D11_MAPPED_SUBRESOURCE mapped_data = {};

		ThrowIfFailed(device_context.Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_data),
					  "Failed to std::map structured buffer");

		memcpy(mapped_data.pData, data.data(), sizeof(DataT) * current_size);

		device_context.Unmap(buffer.Get(), NULL);
	}


	// Bind the buffer to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) {
		StageT::bindSRV(device_context, slot, srv.Get());
	}


	// Get the number of elements this buffer currently holds
	[[nodiscard]]
	u32 size() const {
		return current_size;
	}

	// Get the max number of elements this buffer can currently hold
	[[nodiscard]]
	u32 reserved() const {
		return reserved_size;
	}

private:

	void createBuffer(ID3D11Device& device) {
		// Create the buffer
		D3D11_BUFFER_DESC desc = {};

		desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.Usage               = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth           = sizeof(DataT) * reserved_size;
		desc.StructureByteStride = sizeof(DataT);

		ThrowIfFailed(device.CreateBuffer(&desc, nullptr, buffer.ReleaseAndGetAddressOf()),
					  "Failed to create structured buffer");

		SetDebugObjectName(buffer.Get(), "Structured Buffer");


		// Create the SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Buffer.FirstElement             = 0;
		srv_desc.Buffer.NumElements              = reserved_size;
		srv_desc.Format                          = DXGI_FORMAT_UNKNOWN;
		srv_desc.ViewDimension                   = D3D11_SRV_DIMENSION_BUFFER;

		ThrowIfFailed(device.CreateShaderResourceView(buffer.Get(), &srv_desc, srv.ReleaseAndGetAddressOf()),
					  "Failed to create structured buffer SRV");

		SetDebugObjectName(srv.Get(), "Structured Buffer SRV");
	}


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11Buffer> buffer;
	ComPtr<ID3D11ShaderResourceView> srv;

	// Number of elements in the buffer
	u32 current_size;

	// Max elements buffer can hold
	u32 reserved_size;
};

} //namespace render

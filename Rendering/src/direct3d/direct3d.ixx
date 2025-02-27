module;

#include "datatypes/types.h"

#include "directx/directxtk.h"
#include "directx/d3d11.h"

export module rendering:direct3d;

import exception;
import :display_config;


export namespace render {

class Direct3D final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Direct3D(DisplayConfig& display_config) {
		u32 create_device_flags = 0;
		#if defined(DEBUG) || defined(_DEBUG)
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		D3D_FEATURE_LEVEL feature_level;
		const HRESULT hr = D3D11CreateDevice(display_config.getAdapter(),
											 D3D_DRIVER_TYPE_UNKNOWN,
											 nullptr,      // no software device
											 create_device_flags,
											 nullptr,
											 0,            // default feature level array
											 D3D11_SDK_VERSION,
											 device.GetAddressOf(),
											 &feature_level,
											 device_context.GetAddressOf());

		ThrowIfFailed(hr, "D3D11CreateDevice Failed");

		ThrowIfFailed(feature_level == D3D_FEATURE_LEVEL_11_0,
					  "Direct3D Feature Level 11 unsupported");

		// Create debug object
		#if defined(DEBUG) || defined(_DEBUG)
		device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(debug.GetAddressOf()));
		#endif
	}

	Direct3D(const Direct3D& d3d) = delete;
	Direct3D(Direct3D&& d3d) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Direct3D() {
		// Restore all default settings
		if (device_context) {
			device_context->ClearState();
			device_context->Flush();
		}

		// Report any live objects
		#if defined(DEBUG) || defined(_DEBUG)
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		#endif
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Direct3D& operator=(const Direct3D& d3d) = delete;
	Direct3D& operator=(Direct3D&& d3d) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	ID3D11Device& getDevice() {
		return *device.Get();
	}

	[[nodiscard]]
	ID3D11DeviceContext& getDeviceContext() {
		return *device_context.Get();
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> device_context;
	ComPtr<ID3D11Debug> debug;
};

} //namespace render

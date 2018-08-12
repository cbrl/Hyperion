#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "display/display_config.h"


class Direct3D final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Direct3D(DisplayConfig& display_config);
	Direct3D(const Direct3D& d3d) = delete;
	Direct3D(Direct3D&& d3d) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Direct3D();


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

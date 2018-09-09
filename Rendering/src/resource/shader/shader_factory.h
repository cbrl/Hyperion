#pragma once

#include "datatypes/datatypes.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader.h"


enum class FalseColor {
	Static,
	Normal,
	Depth
};


namespace ShaderFactory {
	
	// Forward
	shared_ptr<PixelShader> createForwardPS(ResourceMgr& resource_mgr, bool transparency);
	shared_ptr<PixelShader> createForwardUnlitPS(ResourceMgr& resource_mgr, bool transparency);
	shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr);

	// Depth
	shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr);
	shared_ptr<PixelShader> createDepthTransparentPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createDepthTransparentVS(ResourceMgr& resource_mgr);

	// Sky
	shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr);

	// Bounding Volume
	shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr);

	// False Color
	shared_ptr<PixelShader> createFalseColorPS(ResourceMgr& resource_mgr, FalseColor color);
}
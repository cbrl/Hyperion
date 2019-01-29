#pragma once

#include "datatypes/datatypes.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader.h"
#include "rendering_options.h"


enum class FalseColor {
	Static,
	Fullbright,
	Normal,
	Depth
};


namespace ShaderFactory {
	
// Forward
std::shared_ptr<PixelShader> CreateForwardPS(ResourceMgr& resource_mgr, BRDF brdf, bool transparency);
std::shared_ptr<VertexShader> CreateForwardVS(ResourceMgr& resource_mgr);

// Depth
std::shared_ptr<VertexShader> CreateDepthVS(ResourceMgr& resource_mgr);
std::shared_ptr<PixelShader> CreateDepthTransparentPS(ResourceMgr& resource_mgr);
std::shared_ptr<VertexShader> CreateDepthTransparentVS(ResourceMgr& resource_mgr);

// Sky
std::shared_ptr<PixelShader> CreateSkyPS(ResourceMgr& resource_mgr);
std::shared_ptr<VertexShader> CreateSkyVS(ResourceMgr& resource_mgr);

// Bounding Volume
std::shared_ptr<PixelShader> CreateWireframeBoxPS(ResourceMgr& resource_mgr);
std::shared_ptr<VertexShader> CreateWireframeBoxVS(ResourceMgr& resource_mgr);

// False Color
std::shared_ptr<PixelShader> CreateFalseColorPS(ResourceMgr& resource_mgr, FalseColor color);

} //namespace ShaderFactory
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
	std::shared_ptr<PixelShader> createForwardPS(ResourceMgr& resource_mgr, BRDF brdf, bool transparency);
    std::shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr);

	// Depth
    std::shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr);
    std::shared_ptr<PixelShader> createDepthTransparentPS(ResourceMgr& resource_mgr);
    std::shared_ptr<VertexShader> createDepthTransparentVS(ResourceMgr& resource_mgr);

	// Sky
    std::shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr);
    std::shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr);

	// Bounding Volume
    std::shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr);
    std::shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr);

	// False Color
    std::shared_ptr<PixelShader> createFalseColorPS(ResourceMgr& resource_mgr, FalseColor color);
}
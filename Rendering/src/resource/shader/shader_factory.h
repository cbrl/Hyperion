#pragma once

#include "resource/resource_mgr.h"
#include "resource/shader/shader.h"


namespace ShaderFactory {
	
	// Forward
	shared_ptr<PixelShader> createForwardPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr);

	// Depth
	//shared_ptr<PixelShader> createDepthPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr);

	// Sky
	shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr);

	// Bounding Volume
	shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr);
	shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr);
}
#include "shader_factory.h"

// Forward
#include "compiled_headers/forward.h"
#include "compiled_headers/forward_vs.h"

// Depth
//#include "compiled_headers/depth.h"
#include "compiled_headers/depth_vs.h"

// Sky
#include "compiled_headers/skybox.h"
#include "compiled_headers/skybox_vs.h"

// Bounding Volume
#include "compiled_headers/wireframe_box_vs.h"
#include "compiled_headers/wireframe_box_ps.h"


namespace ShaderFactory {

	//----------------------------------------------------------------------------------
	// Forward
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createForwardPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_ps", ShaderBytecodeBuffer(shader_forward, sizeof(shader_forward)));
	}

	shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_forward_vs",
													  ShaderBytecodeBuffer(shader_forward_vs, sizeof(shader_forward_vs)),
		                                              VertexPositionNormalTexture::InputElements,
		                                              VertexPositionNormalTexture::InputElementCount);
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	//shared_ptr<PixelShader> createDepthPS(ResourceMgr& resource_mgr) {

	//	return resource_mgr.getOrCreate<PixelShader>(L"shader_depth_ps", ShaderBytecodeBuffer(shader_depth_ps, sizeof(shader_depth_ps)));
	//}

	shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_depth_vs",
													  ShaderBytecodeBuffer(shader_depth_vs, sizeof(shader_depth_vs)),
		                                              VertexPositionNormalTexture::InputElements,
		                                              VertexPositionNormalTexture::InputElementCount);
	}


	//----------------------------------------------------------------------------------
	// Sky
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_skybox_ps", ShaderBytecodeBuffer(shader_skybox, sizeof(shader_skybox)));
	}

	shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_skybox_vs",
													  ShaderBytecodeBuffer(shader_skybox_vs, sizeof(shader_skybox_vs)),
		                                              VertexPositionTexture::InputElements,
		                                              VertexPositionTexture::InputElementCount);
	}


	//----------------------------------------------------------------------------------
	// Bounding Volume
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_wireframe_box_ps",
		                                             ShaderBytecodeBuffer(shader_wireframe_box_ps, sizeof(shader_wireframe_box_ps)));
	}

	shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_wireframe_box_vs",
													  ShaderBytecodeBuffer(shader_wireframe_box_vs, sizeof(shader_wireframe_box_vs)),
		                                              VertexPosition::InputElements,
		                                              VertexPosition::InputElementCount);
	}
}

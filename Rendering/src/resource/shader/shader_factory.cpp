#include "shader_factory.h"

#define BYTECODE(x) ShaderBytecodeBuffer(x, sizeof(x))

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

		return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_ps", BYTECODE(shader_forward));
	}

	shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_forward_vs",
		                                              BYTECODE(shader_forward_vs),
		                                              VertexPositionNormalTexture::InputElements,
		                                              VertexPositionNormalTexture::InputElementCount);
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	//shared_ptr<PixelShader> createDepthPS(ResourceMgr& resource_mgr) {

	//	return resource_mgr.getOrCreate<PixelShader>(L"shader_depth_ps", BYTECODE(shader_depth_ps));
	//}

	shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_depth_vs",
		                                              BYTECODE(shader_depth_vs),
		                                              VertexPositionNormalTexture::InputElements,
		                                              VertexPositionNormalTexture::InputElementCount);
	}


	//----------------------------------------------------------------------------------
	// Sky
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_skybox_ps", BYTECODE(shader_skybox));
	}

	shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_skybox_vs",
		                                              BYTECODE(shader_skybox_vs),
		                                              VertexPositionNormalTexture::InputElements,
		                                              VertexPositionNormalTexture::InputElementCount);
	}


	//----------------------------------------------------------------------------------
	// Bounding Volume
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_wireframe_box_ps", BYTECODE(shader_wireframe_box_ps));
	}

	shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_wireframe_box_vs",
		                                              BYTECODE(shader_wireframe_box_vs),
		                                              VertexPositionNormalTexture::InputElements,
		                                              VertexPositionNormalTexture::InputElementCount);
	}
}

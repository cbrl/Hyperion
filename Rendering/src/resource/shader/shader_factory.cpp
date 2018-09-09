#include "shader_factory.h"

#define BYTECODE(x) ShaderBytecodeBuffer(gsl::make_span(x, sizeof(x)))
//#define BYTECODE(x) ShaderBytecodeBlob("./shaders/"#x)

// Forward
#include "compiled_headers/forward_lit.h"
#include "compiled_headers/forward_transparent_lit.h"
#include "compiled_headers/forward_unlit.h"
#include "compiled_headers/forward_transparent_unlit.h"
#include "compiled_headers/forward_vs.h"

// Depth
#include "compiled_headers/depth_vs.h"
#include "compiled_headers/depth_transparent_ps.h"
#include "compiled_headers/depth_transparent_vs.h"

// Sky
#include "compiled_headers/skybox_ps.h"
#include "compiled_headers/skybox_vs.h"

// Bounding Volume
#include "compiled_headers/wireframe_box_vs.h"
#include "compiled_headers/wireframe_box_ps.h"

// False Color
#include "compiled_headers/static_color.h"
#include "compiled_headers/normal_color.h"
#include "compiled_headers/depth_color.h"


namespace ShaderFactory {

	//----------------------------------------------------------------------------------
	// Forward
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createForwardPS(ResourceMgr& resource_mgr, bool transparency) {

		if (transparency) {
			return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_transparent_lit_ps", BYTECODE(shader_forward_transparent_lit));
		}

		return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_lit_ps", BYTECODE(shader_forward_lit));
	}

	shared_ptr<PixelShader> createForwardUnlitPS(ResourceMgr& resource_mgr, bool transparency) {

		if (transparency) {
			return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_transparent_unlit_ps", BYTECODE(shader_forward_transparent_unlit));
		}

		return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_unlit_ps", BYTECODE(shader_forward_unlit));
	}

	shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_forward_vs",
													  BYTECODE(shader_forward_vs),
		                                              gsl::make_span(VertexPositionNormalTexture::InputElements,
		                                                             VertexPositionNormalTexture::InputElementCount));
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_depth_vs",
													  BYTECODE(shader_depth_vs),
													  gsl::make_span(VertexPositionNormalTexture::InputElements,
													                 VertexPositionNormalTexture::InputElementCount));
	}

	shared_ptr<PixelShader> createDepthTransparentPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_depth_transparent_ps", BYTECODE(shader_depth_transparent_ps));
	}

	shared_ptr<VertexShader> createDepthTransparentVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_depth_transparent_vs",
													  BYTECODE(shader_depth_transparent_vs),
													  gsl::make_span(VertexPositionNormalTexture::InputElements,
													                 VertexPositionNormalTexture::InputElementCount));
	}


	//----------------------------------------------------------------------------------
	// Sky
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_skybox_ps", BYTECODE(shader_skybox_ps));
	}

	shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_skybox_vs",
													  BYTECODE(shader_skybox_vs),
		                                              gsl::make_span(VertexPositionTexture::InputElements,
		                                                             VertexPositionTexture::InputElementCount));
	}


	//----------------------------------------------------------------------------------
	// Bounding Volume
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_wireframe_box_ps",
		                                             BYTECODE(shader_wireframe_box_ps));
	}

	shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_wireframe_box_vs",
													  BYTECODE(shader_wireframe_box_vs),
		                                              gsl::make_span(VertexPosition::InputElements,
		                                                             VertexPosition::InputElementCount));
	}


	//----------------------------------------------------------------------------------
	// False Color
	//----------------------------------------------------------------------------------

	shared_ptr<PixelShader> createFalseColorPS(ResourceMgr& resource_mgr, FalseColor color) {

		switch (color) {
			case FalseColor::Static:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_static", BYTECODE(shader_static_color));

			case FalseColor::Normal:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_normal", BYTECODE(shader_normal_color));

			case FalseColor::Depth:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_depth", BYTECODE(shader_depth_color));

			default:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_static", BYTECODE(shader_static_color));
		}
	}
}

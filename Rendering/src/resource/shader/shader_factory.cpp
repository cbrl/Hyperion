#include "shader_factory.h"

#define BYTECODE(x) ShaderBytecodeBuffer(gsl::make_span(x, sizeof(x)))
//#define BYTECODE(x) ShaderBytecodeBlob("./shaders/"#x)

// Forward
#include "compiled_headers/forward_vs.h"

#include "compiled_headers/forward_lambert.h"
#include "compiled_headers/forward_blinn_phong.h"
#include "compiled_headers/forward_cook_torrance.h"

#include "compiled_headers/forward_transparent_lambert.h"
#include "compiled_headers/forward_transparent_blinn_phong.h"
#include "compiled_headers/forward_transparent_cook_torrance.h"

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
#include "compiled_headers/fullbright.h"
#include "compiled_headers/normal_color.h"
#include "compiled_headers/depth_color.h"


namespace ShaderFactory {

	//----------------------------------------------------------------------------------
	// Forward
	//----------------------------------------------------------------------------------

	std::shared_ptr<PixelShader> createForwardPS(ResourceMgr& resource_mgr, BRDF brdf, bool transparency) {

		switch (brdf) {
		    case BRDF::Lambert: {
			    if (transparency)
				    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_transparent_lambert_ps", BYTECODE(shader_forward_transparent_lambert));
			    else
				    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_lambert_ps", BYTECODE(shader_forward_lambert));
		    }
		    case BRDF::BlinnPhong: {
			    if (transparency)
				    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_transparent_blinn_phong_ps", BYTECODE(shader_forward_transparent_blinn_phong));
			    else
				    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_blinn_phong_ps", BYTECODE(shader_forward_blinn_phong));
		    }
		    case BRDF::CookTorrance: {
			    if (transparency)
				    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_transparent_cook_torrance_ps", BYTECODE(shader_forward_transparent_cook_torrance));
			    else
				    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_cook_torrance_ps", BYTECODE(shader_forward_cook_torrance));
		    }
			default: {
			    return resource_mgr.getOrCreate<PixelShader>(L"shader_forward_lambert_ps", BYTECODE(shader_forward_lambert));
			}
		}
	}

	std::shared_ptr<VertexShader> createForwardVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_forward_vs",
													  BYTECODE(shader_forward_vs),
		                                              gsl::make_span(VertexPositionNormalTexture::input_elements,
		                                                             VertexPositionNormalTexture::input_element_count));
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	std::shared_ptr<VertexShader> createDepthVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_depth_vs",
													  BYTECODE(shader_depth_vs),
													  gsl::make_span(VertexPositionNormalTexture::input_elements,
													                 VertexPositionNormalTexture::input_element_count));
	}

	std::shared_ptr<PixelShader> createDepthTransparentPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_depth_transparent_ps", BYTECODE(shader_depth_transparent_ps));
	}

	std::shared_ptr<VertexShader> createDepthTransparentVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_depth_transparent_vs",
													  BYTECODE(shader_depth_transparent_vs),
													  gsl::make_span(VertexPositionNormalTexture::input_elements,
													                 VertexPositionNormalTexture::input_element_count));
	}


	//----------------------------------------------------------------------------------
	// Sky
	//----------------------------------------------------------------------------------

	std::shared_ptr<PixelShader> createSkyPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_skybox_ps", BYTECODE(shader_skybox_ps));
	}

	std::shared_ptr<VertexShader> createSkyVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_skybox_vs",
													  BYTECODE(shader_skybox_vs),
		                                              gsl::make_span(VertexPositionTexture::input_elements,
		                                                             VertexPositionTexture::input_element_count));
	}


	//----------------------------------------------------------------------------------
	// Bounding Volume
	//----------------------------------------------------------------------------------

	std::shared_ptr<PixelShader> createWireframeBoxPS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<PixelShader>(L"shader_wireframe_box_ps",
		                                             BYTECODE(shader_wireframe_box_ps));
	}

	std::shared_ptr<VertexShader> createWireframeBoxVS(ResourceMgr& resource_mgr) {

		return resource_mgr.getOrCreate<VertexShader>(L"shader_wireframe_box_vs",
													  BYTECODE(shader_wireframe_box_vs),
		                                              gsl::make_span(VertexPosition::input_elements,
		                                                             VertexPosition::input_element_count));
	}


	//----------------------------------------------------------------------------------
	// False Color
	//----------------------------------------------------------------------------------

	std::shared_ptr<PixelShader> createFalseColorPS(ResourceMgr& resource_mgr, FalseColor color) {

		switch (color) {
			case FalseColor::Static:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_static", BYTECODE(shader_static_color));

			case FalseColor::Fullbright:
			    return resource_mgr.getOrCreate <PixelShader>(L"shader_false_color_fullbright", BYTECODE(shader_fullbright));

			case FalseColor::Normal:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_normal", BYTECODE(shader_normal_color));

			case FalseColor::Depth:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_depth", BYTECODE(shader_depth_color));

			default:
				return resource_mgr.getOrCreate<PixelShader>(L"shader_false_color_static", BYTECODE(shader_static_color));
		}
	}
}

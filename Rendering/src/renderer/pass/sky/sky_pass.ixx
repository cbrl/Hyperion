module;

#include "datatypes/types.h"
#include "hlsl.h"

#include "directx/d3d11.h"

export module rendering:pass.sky_pass;

import :pipeline;
import :render_state_mgr;
import :resource_mgr;
import :shader;
import :shader_factory;
import :texture;


namespace render {

export class SkyPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SkyPass(ID3D11DeviceContext& device_context,
	        RenderStateMgr& render_state_mgr,
	        ResourceMgr& resource_mgr)
		: device_context(device_context)
		, render_state_mgr(render_state_mgr) {

		vertex_shader = ShaderFactory::CreateSkyVS(resource_mgr);
		pixel_shader  = ShaderFactory::CreateSkyPS(resource_mgr);
	}

	SkyPass(const SkyPass&) = delete;
	SkyPass(SkyPass&&) = default;

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~SkyPass() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SkyPass& operator=(const SkyPass&) = delete;
	SkyPass& operator=(SkyPass&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void render(const Texture* sky) const {
		if (!sky) return;

		// Bind the render states
		bindRenderStates();

		// Bind the texture
		sky->bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);

		// Render the skybox
		Pipeline::draw(device_context, 384, 0);
	}

private:

	void bindRenderStates() const {
		// Bind null vertex/index buffer (skybox shader uses its own vertex array)
		Pipeline::IA::bindIndexBuffer(device_context, nullptr, DXGI_FORMAT_UNKNOWN, 0);
		Pipeline::IA::bindVertexBuffer(device_context, 0, nullptr, 0, 0);

		Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pipeline::DS::bindShader(device_context, nullptr, {});
		Pipeline::GS::bindShader(device_context, nullptr, {});
		Pipeline::HS::bindShader(device_context, nullptr, {});

		render_state_mgr.bind(device_context, BlendStates::Opaque);
		render_state_mgr.bind(device_context, DepthStencilStates::LessEqR);
		render_state_mgr.bind(device_context, RasterStates::CullCounterClockwise);

		vertex_shader->bind(device_context);
		pixel_shader->bind(device_context);
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	std::shared_ptr<VertexShader> vertex_shader;
	std::shared_ptr<PixelShader>  pixel_shader;
};

} //namespace render

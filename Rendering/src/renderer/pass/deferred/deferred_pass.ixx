module;

#include "datatypes/types.h"
#include "memory/handle/handle.h"

#include "directx/d3d11.h"
#include "directx/directxtk.h"

export module rendering:pass.deferred_pass;

import :pipeline;
import :render_state_mgr;
import :rendering_options;
import :resource_mgr;
import :shader_factory;


namespace render {

export class DeferredPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DeferredPass(ID3D11DeviceContext& device_context,
	             RenderStateMgr& render_state_mgr,
	             ResourceMgr& resource_mgr)
		: device_context(device_context)
		, render_state_mgr(render_state_mgr)
		, resource_mgr(resource_mgr) {

		vertex_shader = ShaderFactory::CreateFullscreenQuadVS(resource_mgr);
	}

	DeferredPass(const DeferredPass&) = delete;
	DeferredPass(DeferredPass&&) = default;

	
	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~DeferredPass() = default;

	
	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	DeferredPass& operator=(const DeferredPass&) = delete;
	DeferredPass& operator=(DeferredPass&&) = default;

	
	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void render(BRDF brdf) const {
		// Bind the render states and vertex shader
		bindState();

		// Create the appropriate BRDF pixel shader
		const auto ps = ShaderFactory::CreateDeferredPS(resource_mgr, brdf);
		ps->bind(device_context);

		// Draw a fullscreen quad
		Pipeline::draw(device_context, 6, 0);
	}

private:

	void bindState() const {
		// Bind topology
		Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind vertex shader
		vertex_shader->bind(device_context);

		// Unbind shaders
		Pipeline::DS::bindShader(device_context, nullptr, {});
		Pipeline::GS::bindShader(device_context, nullptr, {});
		Pipeline::HS::bindShader(device_context, nullptr, {});

		// Bind render states
		render_state_mgr.get().bind(device_context, BlendStates::Opaque);
		render_state_mgr.get().bind(device_context, DepthStencilStates::None);
		render_state_mgr.get().bind(device_context, RasterStates::CullCounterClockwise);
	}

	
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr> render_state_mgr;
	std::reference_wrapper<ResourceMgr> resource_mgr;

	std::shared_ptr<VertexShader> vertex_shader;
};

} //namespace render

#include "deferred_pass.h"
#include "renderer/state/render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader_factory.h"


namespace render {

DeferredPass::DeferredPass(ID3D11DeviceContext& device_context,
                           RenderStateMgr& render_state_mgr,
                           ResourceMgr& resource_mgr)
	: device_context(device_context)
	, render_state_mgr(render_state_mgr)
	, resource_mgr(resource_mgr) {

	vertex_shader = ShaderFactory::CreateFullscreenQuadVS(resource_mgr);
}


void DeferredPass::bindState() const {

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


void DeferredPass::render(BRDF brdf) const {
	
	// Bind the render states and vertex shader
	bindState();

	// Create the appropriate BRDF pixel shader
	const auto ps = ShaderFactory::CreateDeferredPS(resource_mgr, brdf);
	ps->bind(device_context);
	
	// Draw a fullscreen quad
	Pipeline::draw(device_context, 6, 0);
}

} //namespace render
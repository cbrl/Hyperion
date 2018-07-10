#include "sky_pass.h"

#include "engine/engine.h"
#include "hlsl.h"
#include "resource/shader/shader_factory.h"


SkyPass::SkyPass(ID3D11Device& device,
                 ID3D11DeviceContext& device_context,
                 RenderStateMgr& render_state_mgr,
                 ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context)
	, render_state_mgr(render_state_mgr) {

	vertex_shader = ShaderFactory::createSkyVS(resource_mgr);
	pixel_shader  = ShaderFactory::createSkyPS(resource_mgr);
}


void SkyPass::render(const Texture* sky) const {

	if (!sky) return;

	// Bind the render states
	bindRenderStates();

	// Bind the texture
	sky->bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);

	// Render the skybox
	Pipeline::draw(device_context, 384, 0);
}


void SkyPass::bindRenderStates() const {

	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

	render_state_mgr.get().bindOpaque(device_context);
	render_state_mgr.get().bindDepthLessEqRead(device_context);
	render_state_mgr.get().bindCullCounterClockwise(device_context);

	vertex_shader->bind(device_context);
	pixel_shader->bind(device_context);
}

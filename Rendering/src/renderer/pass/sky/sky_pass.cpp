#include "sky_pass.h"
#include "engine/engine.h"
#include "hlsl.h"

#include "compiled_headers/skybox.h"
#include "compiled_headers/skybox_vs.h"


SkyPass::SkyPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context) {

	vertex_shader = make_unique<VertexShader>(device,
	                                          shader_skybox_vs,
	                                          sizeof(shader_skybox_vs),
	                                          VertexPosition::InputElements,
	                                          VertexPosition::InputElementCount);

	pixel_shader = make_unique<PixelShader>(device, shader_skybox, sizeof(shader_skybox));
}


void SkyPass::render(const Engine& engine, const SkyBox& skybox) const {

	// Bind the render states
	bindRenderStates(engine.getRenderingMgr().getRenderStateMgr());

	// Bind the texture
	const auto texture = skybox.getTexture();
	if (texture) texture->bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);

	// Render the skybox
	skybox.bind(device_context);
	skybox.draw(device_context);
}


void SkyPass::bindRenderStates(const RenderStateMgr& render_state_mgr) const {

	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

	render_state_mgr.bindOpaque(device_context);
	render_state_mgr.bindDepthDefault(device_context);
	render_state_mgr.bindCullNone(device_context);

	vertex_shader->bind(device_context);
	pixel_shader->bind(device_context);
}

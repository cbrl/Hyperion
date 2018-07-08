#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "resource/texture/texture.h"

class Engine;
class RenderStateMgr;

class SkyPass final {
public:
	SkyPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~SkyPass() = default;

	void render(const Engine& engine, const Texture* sky) const;


private:
	void bindRenderStates(const RenderStateMgr& render_state_mgr) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<VertexShader> vertex_shader;
	unique_ptr<PixelShader> pixel_shader;
};

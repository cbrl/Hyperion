#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "resource/texture/texture.h"

class Engine;
class RenderStateMgr;
class ResourceMgr;

class SkyPass final {
public:
	SkyPass(ID3D11DeviceContext& device_context,
	        RenderStateMgr& render_state_mgr,
	        ResourceMgr& resource_mgr);
	~SkyPass() = default;

	void render(const Texture* sky) const;


private:
	void bindRenderStates() const;


private:
	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr> render_state_mgr;

	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
};

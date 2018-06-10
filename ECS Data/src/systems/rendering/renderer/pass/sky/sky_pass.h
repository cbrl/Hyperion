#pragma once

#include <d3d11.h>

#include "datatypes/datatypes.h"
#include "hlsl.h"
#include "shader/shader.h"
#include "components/rendering/skybox/skybox.h"

class ECS;
class Scene;
class RenderStateMgr;


class SkyPass final {
public:
	SkyPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~SkyPass() = default;

	void render(const SkyBox& skybox) const;
	void bindDefaultRenderStates(const RenderStateMgr& render_state_mgr) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<VertexShader> vertex_shader;
	unique_ptr<PixelShader> pixel_shader;
};

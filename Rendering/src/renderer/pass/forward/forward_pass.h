#pragma once

#include "datatypes/datatypes.h"
#include "pipeline.h"
#include "buffer/buffers.h"
#include "hlsl.h"
#include "shader/shader.h"
#include "components/rendering/model/model.h"
#include "components/rendering/camera/skybox/skybox.h"

class Engine;
class RenderStateMgr;
class ECS;

class ForwardPass final {
public:
	ForwardPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~ForwardPass() = default;

	void XM_CALLCONV render(const Engine& engine, FXMMATRIX world_to_projection) const;
	void XM_CALLCONV render(const Engine& engine, const SkyBox& sky, FXMMATRIX world_to_projection) const;


private:
	void bindRenderStates(const RenderStateMgr& render_state_mgr) const;
	void XM_CALLCONV render(ECS& ecs_engine, Model& model, FXMMATRIX world_to_projection) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<PixelShader> pixel_shader;
	unique_ptr<VertexShader> vertex_shader;
};

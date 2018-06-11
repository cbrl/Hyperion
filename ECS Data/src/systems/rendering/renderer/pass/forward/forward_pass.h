#pragma once

#include "os/d3d11.h"

#include "datatypes/datatypes.h"
#include "math/math.h"
#include "pipeline.h"
#include "buffer/buffers.h"
#include "hlsl.h"
#include "shader/shader.h"
#include "components/rendering/model/model.h"

class ECS;
class Scene;
class RenderStateMgr;


class ForwardPass final {
public:
	ForwardPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~ForwardPass() = default;

	void XM_CALLCONV render(ECS& ecs_engine, FXMMATRIX world_to_projection) const;

	void XM_CALLCONV render(const ECS& ecs_engine, Model& model, FXMMATRIX world_to_projection) const;

	void bindDefaultRenderStates(const RenderStateMgr& render_state_mgr) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<PixelShader> pixel_shader;
	unique_ptr<VertexShader> vertex_shader;
};

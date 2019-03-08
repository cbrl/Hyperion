#pragma once

#include "datatypes/datatypes.h"
#include "directx/d3d11.h"
#include "directx/directxtk.h"
#include "resource/shader/shader.h"
#include "rendering_options.h"

class RenderStateMgr;
class ResourceMgr;
class Scene;
class Model;

class DeferredPass final {
public:
	DeferredPass(ID3D11DeviceContext& device_context,
	             RenderStateMgr& render_state_mgr,
	             ResourceMgr& resource_mgr);

	DeferredPass(const DeferredPass&) = delete;
	DeferredPass(DeferredPass&&) = default;

	~DeferredPass() = default;

	DeferredPass& operator=(const DeferredPass&) = delete;
	DeferredPass& operator=(DeferredPass&&) = default;

	void render(BRDF brdf) const;

private:

	void bindState() const;

	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr> render_state_mgr;
	std::reference_wrapper<ResourceMgr> resource_mgr;

	std::shared_ptr<VertexShader> vertex_shader;
};
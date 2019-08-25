#pragma once

#include "datatypes/types.h"
#include "directx/d3d11.h"
#include "directx/directxtk.h"
#include "resource/shader/shader.h"
#include "rendering_options.h"

namespace render {

class RenderStateMgr;
class ResourceMgr;

class DeferredPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DeferredPass(ID3D11DeviceContext& device_context,
	             RenderStateMgr& render_state_mgr,
	             ResourceMgr& resource_mgr);

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
	void render(BRDF brdf) const;

private:

	void bindState() const;

	
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr> render_state_mgr;
	std::reference_wrapper<ResourceMgr> resource_mgr;

	std::shared_ptr<VertexShader> vertex_shader;
};

} //namespace render

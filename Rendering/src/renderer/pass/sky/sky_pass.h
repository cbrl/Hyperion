#pragma once

#include "datatypes/types.h"
#include "resource/shader/shader.h"
#include "resource/texture/texture.h"


namespace render {

class RenderStateMgr;
class ResourceMgr;

class SkyPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SkyPass(ID3D11DeviceContext& device_context,
	        RenderStateMgr& render_state_mgr,
	        ResourceMgr& resource_mgr);

	SkyPass(const SkyPass&) = delete;
	SkyPass(SkyPass&&) = default;

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~SkyPass() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SkyPass& operator=(const SkyPass&) = delete;
	SkyPass& operator=(SkyPass&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void render(const Texture* sky) const;

private:

	void bindRenderStates() const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	std::shared_ptr<VertexShader> vertex_shader;
	std::shared_ptr<PixelShader>  pixel_shader;
};

} //namespace render

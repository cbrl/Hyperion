module;

#include "datatypes/types.h"

#include "directx/d3d11.h"
#include "directxmath/directxmath.h"
#include "rendering_options.h"

export module rendering.pass.forward_pass;

import rendering.constant_buffer;
import rendering.pipeline;
import rendering.shader;


class Model;
class Transform;

namespace ecs {
	class ECS;
}

namespace render {

class RenderStateMgr;
class ResourceMgr;
class Texture;

export class ForwardPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ForwardPass(ID3D11Device& device,
	            ID3D11DeviceContext& device_context,
	            RenderStateMgr& render_state_mgr,
	            ResourceMgr& resource_mgr);

	ForwardPass(const ForwardPass& pass) = delete;
	ForwardPass(ForwardPass&& pass) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ForwardPass() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ForwardPass& operator=(const ForwardPass& pass) = delete;
	ForwardPass& operator=(ForwardPass&& pass) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions - Render With Specified Mode
	//----------------------------------------------------------------------------------

	// Render all (opaque) models with a given BRDF
	void XM_CALLCONV renderOpaque(const ecs::ECS& ecs,
	                              FXMMATRIX world_to_projection,
	                              const Texture* env_map,
	                              BRDF brdf) const;

	// Render all (transparent) models with a given BRDF
	void XM_CALLCONV renderTransparent(const ecs::ECS& ecs,
	                                   FXMMATRIX world_to_projection,
	                                   const Texture* env_map,
	                                   BRDF brdf) const;

	// Render all models with the given false color mode
	void XM_CALLCONV renderFalseColor(const ecs::ECS& ecs,
	                                  FXMMATRIX world_to_projection,
	                                  FalseColor color) const;

	// Render all models as a wireframe
	void XM_CALLCONV renderWireframe(const ecs::ECS& ecs,
	                                 FXMMATRIX world_to_projection,
	                                 const f32_4& color) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Render With Overrided Shader
	//----------------------------------------------------------------------------------

	// Sorts by shader type all models with overrided shaders, then renders them.
	// Renders opaque models, then transparent. Call between opaque and transparent render passes.
	void XM_CALLCONV renderOverrided(const ecs::ECS& ecs,
	                                 FXMMATRIX world_to_projection,
	                                 const Texture* env_map) const;

	
	//----------------------------------------------------------------------------------
	// Member Functions - Render to GBuffer
	//----------------------------------------------------------------------------------
	void XM_CALLCONV renderGBuffer(const ecs::ECS& ecs, FXMMATRIX world_to_projection) const;

private:

	//----------------------------------------------------------------------------------
	// Member Functions - Bind State
	//----------------------------------------------------------------------------------
	void bindOpaqueState() const;
	void bindTransparentState() const;
	void bindWireframeState() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Render Model
	//----------------------------------------------------------------------------------
	void XM_CALLCONV renderModel(const Model& model, const Transform& transform, FXMMATRIX world_to_projection) const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;
	ResourceMgr&         resource_mgr;

	// Shaders
	std::shared_ptr<VertexShader> vertex_shader;
	std::shared_ptr<PixelShader>  gbuffer_shader;

	// Buffers
	ConstantBuffer<f32_4> color_buffer;
};

} //namespace render

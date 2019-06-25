#pragma once

#include "datatypes/datatypes.h"
#include "buffer/constant_buffer.h"
#include "resource/shader/shader.h"
#include "rendering_options.h"

class Model;
class Transform;

namespace render {

class RenderStateMgr;
class ResourceMgr;
class Scene;
class Texture;

class ForwardPass final {
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
	void XM_CALLCONV renderOpaque(const Scene& scene,
	                              FXMMATRIX world_to_projection,
	                              const Texture* env_map,
	                              BRDF brdf) const;

	// Render all (transparent) models with a given BRDF
	void XM_CALLCONV renderTransparent(const Scene& scene,
	                                   FXMMATRIX world_to_projection,
	                                   const Texture* env_map,
	                                   BRDF brdf) const;

	// Render all models with the given false color mode
	void XM_CALLCONV renderFalseColor(const Scene& scene,
	                                  FXMMATRIX world_to_projection,
	                                  FalseColor color) const;

	// Render all models as a wireframe
	void XM_CALLCONV renderWireframe(const Scene& scene,
	                                 FXMMATRIX world_to_projection,
	                                 const f32_4& color) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Render With Overrided Shader
	//----------------------------------------------------------------------------------

	// Sorts by shader type all models with overrided shaders, then renders them.
	// Renders opaque models, then transparent. Call between opaque and transparent render passes.
	void XM_CALLCONV renderOverrided(const Scene& scene,
	                                 FXMMATRIX world_to_projection,
	                                 const Texture* env_map) const;

	
	//----------------------------------------------------------------------------------
	// Member Functions - Render to GBuffer
	//----------------------------------------------------------------------------------
	void XM_CALLCONV renderGBuffer(const Scene& scene, FXMMATRIX world_to_projection) const;

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

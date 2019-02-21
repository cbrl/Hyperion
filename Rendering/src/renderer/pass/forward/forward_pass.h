#pragma once

#include "datatypes/datatypes.h"
#include "buffer/constant_buffer.h"
#include "resource/shader/shader.h"
#include "resource/shader/shader_factory.h"

class RenderStateMgr;
class ResourceMgr;
class Scene;
class Model;

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
	// Member Functions - Render With Specified Shader
	//----------------------------------------------------------------------------------

	// Render the given models with the given shader (opaque only)
	void XM_CALLCONV renderOpaque(const std::vector<std::reference_wrapper<const Model>>& models,
	                              FXMMATRIX world_to_projection,
	                              const Texture* env_map,
	                              PixelShader* shader) const;

	// Render the given models with the given shader (transparent only)
	void XM_CALLCONV renderTransparent(const std::vector<std::reference_wrapper<const Model>>& models,
	                                   FXMMATRIX world_to_projection,
	                                   const Texture* env_map,
	                                   PixelShader* shader) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Render With Specified Mode
	//----------------------------------------------------------------------------------

	// Render all (opaque) models with a given BRDF
	void XM_CALLCONV renderOpaque(Scene& scene,
	                              FXMMATRIX world_to_projection,
	                              const Texture* env_map,
	                              BRDF brdf) const;

	// Render all (transparent) models with a given BRDF
	void XM_CALLCONV renderTransparent(Scene& scene,
	                                   FXMMATRIX world_to_projection,
	                                   const Texture* env_map,
	                                   BRDF brdf) const;

	// Render all models with the given false color mode
	void XM_CALLCONV renderFalseColor(Scene& scene,
	                                  FXMMATRIX world_to_projection,
	                                  FalseColor color) const;

	// Render all models as a wireframe
	void XM_CALLCONV renderWireframe(Scene& scene,
	                                 FXMMATRIX world_to_projection,
	                                 const f32_4& color) const;

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
	void XM_CALLCONV renderModel(const Model& model, FXMMATRIX world_to_projection) const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr& render_state_mgr;
	ResourceMgr& resource_mgr;

	// Shaders
	std::shared_ptr<VertexShader> vertex_shader;

	// Buffers
	ConstantBuffer<f32_4> color_buffer;
};

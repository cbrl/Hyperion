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
	// Member Functions - Render Scene
	//----------------------------------------------------------------------------------

	void XM_CALLCONV renderOpaque(Scene& scene,
	                              FXMMATRIX world_to_projection,
	                              const Texture* sky) const;

	void XM_CALLCONV renderTransparent(Scene& scene,
	                                   FXMMATRIX world_to_projection,
	                                   const Texture* sky) const;

	void XM_CALLCONV renderUnlit(Scene& scene,
	                             FXMMATRIX world_to_projection,
	                             const Texture* sky) const;

	void XM_CALLCONV renderFalseColor(Scene& scene,
	                                  FXMMATRIX world_to_projection,
	                                  FalseColor color) const;

	void XM_CALLCONV renderWireframe(Scene& scene, FXMMATRIX world_to_projection, const vec4_f32& color) const;


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


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr& render_state_mgr;
	ResourceMgr& resource_mgr;

	// Shaders
	shared_ptr<VertexShader> vertex_shader;

	// Buffers
	ConstantBuffer<vec4_f32> color_buffer;
};

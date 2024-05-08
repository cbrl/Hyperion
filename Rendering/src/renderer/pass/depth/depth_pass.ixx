module;

#include "datatypes/types.h"
#include "memory/handle/handle.h"

#include "hlsl.h"

#include "directx/d3d11.h"
#include "directxmath/directxmath_wrapper.h"

export module rendering:pass.depth_pass;

import ecs;

import math.geometry;

import :components.model;
import :components.transform;
import :buffer_types;
import :constant_buffer;
import :pipeline;
import :render_state_mgr;
import :resource_mgr;
import :shader_factory;


namespace render {

export class DepthPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DepthPass(ID3D11Device& device,
	          ID3D11DeviceContext& device_context,
	          RenderStateMgr& render_state_mgr,
	          ResourceMgr& resource_mgr)
		: device_context(device_context)
		, render_state_mgr(render_state_mgr)
		, alt_cam_buffer(device) {

		opaque_vs      = ShaderFactory::CreateDepthVS(resource_mgr);
		transparent_vs = ShaderFactory::CreateDepthTransparentVS(resource_mgr);
		transparent_ps = ShaderFactory::CreateDepthTransparentPS(resource_mgr);
	}

	DepthPass(const DepthPass&) = delete;
	DepthPass(DepthPass&&) = default;
	

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~DepthPass() = default;

	
	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	DepthPass& operator=(const DepthPass&) = delete;
	DepthPass& operator=(DepthPass&&) = default;

	
	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void bindState() const {
		// Bind null shaders
		Pipeline::HS::bindShader(device_context, nullptr, {});
		Pipeline::DS::bindShader(device_context, nullptr, {});
		Pipeline::GS::bindShader(device_context, nullptr, {});

		// Render States
		render_state_mgr.bind(device_context, RasterStates::CullCounterClockwise);
		render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
	}

	void XM_CALLCONV render(const ecs::ECS& ecs,
	                        FXMMATRIX world_to_camera,
	                        CXMMATRIX camera_to_projection) const {
		// Update and bind the camera buffer
		updateCamera(world_to_camera, camera_to_projection);

		// Create the world_to_projection matrix
		const auto world_to_proj = world_to_camera * camera_to_projection;


		//----------------------------------------------------------------------------------
		// Draw each opaque model
		//----------------------------------------------------------------------------------

		ecs.forEach<Model, Transform>([&](handle64 entity) {
			const auto& model     = ecs.get<Model>(entity);
			const auto& transform = ecs.get<Transform>(entity);

			if (not model.isActive())
				return;

			const auto& mat = model.getMaterial();
			if (mat.params.base_color[3] <= ALPHA_MAX)
				return;

			renderModel(model, transform, world_to_proj);
		});


		//----------------------------------------------------------------------------------
		// Draw each transparent model
		//----------------------------------------------------------------------------------

		ecs.forEach<Model, Transform>([&](handle64 entity) {
			const auto& model     = ecs.get<Model>(entity);
			const auto& transform = ecs.get<Transform>(entity);

			if (not model.isActive())
				return;
		
			const auto& mat = model.getMaterial();
			if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
				return;

			renderModel(model, transform, world_to_proj);
		});
	}

	void XM_CALLCONV renderShadows(const ecs::ECS& ecs,
	                               FXMMATRIX world_to_camera,
	                               CXMMATRIX camera_to_projection) const {
		updateCamera(world_to_camera, camera_to_projection);

		const auto world_to_proj = world_to_camera * camera_to_projection;

		//----------------------------------------------------------------------------------
		// Draw each opaque model
		//----------------------------------------------------------------------------------

		bindOpaqueShaders();
		ecs.forEach<Model, Transform>([&](handle64 entity) {
			const auto& model     = ecs.get<Model>(entity);
			const auto& transform = ecs.get<Transform>(entity);

			if (not model.isActive()) return;
			if (not model.castsShadows()) return;

			const auto& mat = model.getMaterial();
			if (mat.params.base_color[3] <= ALPHA_MAX)
				return;

			renderModel(model, transform, world_to_proj);
		});


		//----------------------------------------------------------------------------------
		// Draw each transparent model
		//----------------------------------------------------------------------------------

		bindTransparentShaders();
		ecs.forEach<Model, Transform>([&](handle64 entity) {
			const auto& model     = ecs.get<Model>(entity);
			const auto& transform = ecs.get<Transform>(entity);

			if (not model.isActive()) return;
			if (not model.castsShadows()) return;

			const auto& mat = model.getMaterial();
			if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
				return;

			renderModel(model, transform, world_to_proj);
		});
	}

private:

	void bindOpaqueShaders() const {
		opaque_vs->bind(device_context);
		Pipeline::PS::bindShader(device_context, nullptr, {});
	}

	void bindTransparentShaders() const {
		transparent_vs->bind(device_context);
		transparent_ps->bind(device_context);
	}

	void XM_CALLCONV updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const {
		AltCameraBuffer buffer;
		buffer.world_to_camera      = XMMatrixTranspose(world_to_camera);
		buffer.camera_to_projection = XMMatrixTranspose(camera_to_projection);

		alt_cam_buffer.updateData(device_context, buffer);
		alt_cam_buffer.bind<Pipeline::VS>(device_context, SLOT_CBUFFER_CAMERA_ALT);
	}

	void XM_CALLCONV renderModel(const Model& model, const Transform& transform, FXMMATRIX world_to_projection) const {
		const auto model_to_world      = transform.getObjectToWorldMatrix();
		const auto model_to_projection = model_to_world * world_to_projection;

		if (not Frustum(model_to_projection).contains(model.getAABB()))
			return;

		model.bindMesh(device_context);
		model.bindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);
		model.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);

		const auto& mat = model.getMaterial();
		if (mat.maps.base_color)
			mat.maps.base_color->bind<Pipeline::PS>(device_context, SLOT_SRV_BASE_COLOR);

		Pipeline::drawIndexed(device_context, model.getIndexCount(), 0);

		Pipeline::PS::bindSRV(device_context, SLOT_SRV_BASE_COLOR, nullptr);
	}

	
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	std::shared_ptr<VertexShader> opaque_vs;
	std::shared_ptr<VertexShader> transparent_vs;
	std::shared_ptr<PixelShader>  transparent_ps;

	// Buffers
	ConstantBuffer<AltCameraBuffer> alt_cam_buffer;
};

} //namespace render

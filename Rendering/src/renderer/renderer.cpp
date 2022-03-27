module;

#include <chrono>
#include <memory>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

#include "hlsl.h"

#include "directx/d3d11.h"
#include "directxmath/directxmath.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"


module rendering.renderer;

import rendering.display_config;
import rendering.scene;
import rendering.pass.bounding_volume_pass;
import rendering.pass.deferred_pass;
import rendering.pass.depth_pass;
import rendering.pass.forward_pass;
import rendering.pass.light_pass;
import rendering.pass.sky_pass;
import rendering.pass.text_pass;
import rendering.pipeline;


namespace render {

Renderer::Renderer(DisplayConfig& display_config,
                   const RenderingConfig& rendering_config,
                   ID3D11Device& device,
                   ID3D11DeviceContext& device_context,
                   SwapChain& swap_chain,
                   ResourceMgr& resource_mgr)
	: device(device)
	, device_context(device_context)
	, display_config(display_config)
	, profiler(device, device_context)
	, engine_buffer(device) {

	// Bind the engine buffer (stays bound for the engine's lifetime)
	engine_buffer.bind<Pipeline>(device_context, SLOT_CBUFFER_ENGINE);

	// Create state managers
	output_mgr       = std::make_unique<OutputMgr>(display_config, device, swap_chain);
	render_state_mgr = std::make_unique<RenderStateMgr>(device, device_context);

	// Create renderers
	light_pass           = std::make_unique<LightPass>(rendering_config, device, device_context, *render_state_mgr, resource_mgr);
	forward_pass         = std::make_unique<ForwardPass>(device, device_context, *render_state_mgr, resource_mgr);
	deferred_pass        = std::make_unique<DeferredPass>(device_context, *render_state_mgr, resource_mgr);
	sky_pass             = std::make_unique<SkyPass>(device_context, *render_state_mgr, resource_mgr);
	bounding_volume_pass = std::make_unique<BoundingVolumePass>(device, device_context, *render_state_mgr, resource_mgr);
	text_pass            = std::make_unique<TextPass>(device_context);
}


// This needs to be defined in this implementation file so that the unique_ptr's holding incomplete
// types won't cause a compile error.
Renderer::~Renderer() = default;


void Renderer::render(Scene& scene, std::chrono::duration<f32> delta_time) {

	//----------------------------------------------------------------------------------
	// Update the engine buffer
	//----------------------------------------------------------------------------------
	updateBuffers(delta_time);

	//----------------------------------------------------------------------------------
	// Begin a new profiler frame
	//----------------------------------------------------------------------------------
	profiler.beginFrame();


	profiler.beginTimestamp("Render Scene");
	{
		//----------------------------------------------------------------------------------
		// Bind the initial output state
		//----------------------------------------------------------------------------------
		output_mgr->bindBegin(device_context);

	
		//----------------------------------------------------------------------------------
		// Render the scene for each camera
		//----------------------------------------------------------------------------------
		scene.getECS().forEach<PerspectiveCamera>([&](const PerspectiveCamera& camera) {
			if (not camera.isActive())
				return;

			// Bind the buffer and viewport
			camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
			camera.bindViewport(device_context);

			// Render the scene
			renderCamera(scene, camera);
		});

		scene.getECS().forEach<OrthographicCamera>([&](const OrthographicCamera& camera) {
			if (not camera.isActive())
				return;

			// Bind the buffer and viewport
			camera.bindBuffer(device_context, SLOT_CBUFFER_CAMERA);
			camera.bindViewport(device_context);

			// Render the scene
			renderCamera(scene, camera);
		});


		//----------------------------------------------------------------------------------
		// Bind the final output state
		//----------------------------------------------------------------------------------
		output_mgr->bindEnd(device_context);


		//----------------------------------------------------------------------------------
		// Render text objects
		//----------------------------------------------------------------------------------
		profiler.beginTimestamp("Text");
		text_pass->render(scene.getECS());
		profiler.endTimestamp("Text");
	}
	profiler.endTimestamp("Render Scene");


	//----------------------------------------------------------------------------------
	// Render ImGui data
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("ImGui");
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	profiler.endTimestamp("ImGui");

	
	//----------------------------------------------------------------------------------
	// End profiler frame and update
	//----------------------------------------------------------------------------------
	profiler.endFrame();
	profiler.update();
}


void Renderer::updateBuffers(std::chrono::duration<f32> delta_time) {
	EngineBuffer buffer;

	buffer.resolution = display_config.getDisplayResolution();
	buffer.delta_time = delta_time.count();

	engine_buffer.updateData(device_context, buffer);
}


// This function template is only called from within this translation unit so it can be defined here as well
template<typename CameraT>
void Renderer::renderCamera(Scene& scene, const CameraT& camera) {

	// Camera variables
	const auto& settings  = camera.getSettings();
	const auto* transform = scene.getECS().tryGet<Transform>(camera.getOwner());
	assert(transform != nullptr);

	const auto  world_to_camera      = transform->getWorldToObjectMatrix();
	const auto  camera_to_projection = camera.getCameraToProjectionMatrix();
	const auto  world_to_projection  = world_to_camera * camera_to_projection;
	const auto* skybox               = settings.getSkybox();

	//----------------------------------------------------------------------------------
	// Render the scene
	//----------------------------------------------------------------------------------
	switch (settings.getRenderMode()) {
		case RenderMode::Forward: {
			renderForward(scene, camera, world_to_projection);
			break;
		}
		case RenderMode::ForwardPlus: {
			//renderForwardPlus(scene, camera, world_to_projection);
			break;
		}
		case RenderMode::Deferred: {
			renderDeferred(scene, camera, world_to_projection);
			break;
		}
		case RenderMode::FalseColor: {
			renderFalseColor(scene, camera, world_to_projection);
			break;
		}
		default: break;
	}

	// Bind the forward state
	output_mgr->bindBeginForward(device_context);

	// Render wireframes
	if (settings.hasRenderOption(RenderOptions::Wireframe))
		forward_pass->renderWireframe(scene.getECS(), world_to_projection, camera.getSettings().getWireframeColor());

	// Render bounding volumes
	if (settings.hasRenderOption(RenderOptions::BoundingVolume))
		bounding_volume_pass->render(scene.getECS(), world_to_projection, camera.getSettings().getBoundingVolumeColor());

	// Clear the bound forward state
	output_mgr->bindEndForward(device_context);
}


template<typename CameraT>
void XM_CALLCONV Renderer::renderForward(Scene& scene,
                                         CameraT& camera,
                                         FXMMATRIX world_to_projection) {

	const auto& settings = camera.getSettings();
	const auto* skybox   = settings.getSkybox();

	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Shadow Maps");
	light_pass->render(scene.getECS(), world_to_projection);
	profiler.endTimestamp("Shadow Maps");


	//----------------------------------------------------------------------------------
	// Bind the camera's viewport and the forward output state
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);


	output_mgr->bindBeginForward(device_context);

	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Skybox");
	sky_pass->render(skybox);
	profiler.endTimestamp("Skybox");


	//----------------------------------------------------------------------------------
	// Render the scene
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Forward");

	profiler.beginTimestamp("Opaque");
	forward_pass->renderOpaque(scene.getECS(), world_to_projection, skybox, settings.getBRDF());
	profiler.endTimestamp("Opaque");


	profiler.beginTimestamp("Overrided Shaders");
	forward_pass->renderOverrided(scene.getECS(), world_to_projection, skybox);
	profiler.endTimestamp("Overrided Shaders");


	profiler.beginTimestamp("Transparent");
	forward_pass->renderTransparent(scene.getECS(), world_to_projection, skybox, settings.getBRDF());
	profiler.endTimestamp("Transparent");

	profiler.endTimestamp("Forward");


	output_mgr->bindEndForward(device_context);
}


template<typename CameraT>
void XM_CALLCONV Renderer::renderDeferred(Scene& scene,
                                          CameraT& camera,
                                          FXMMATRIX world_to_projection) {

	const auto& settings = camera.getSettings();
	const auto* skybox = settings.getSkybox();

	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Shadow Maps");
	light_pass->render(scene.getECS(), world_to_projection);
	profiler.endTimestamp("Shadow Maps");


	//----------------------------------------------------------------------------------
	// Bind the camera's viewport
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);


	//----------------------------------------------------------------------------------
	// Render the gbuffer
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("GBuffer");
	output_mgr->bindBeginGBuffer(device_context);
	forward_pass->renderGBuffer(scene.getECS(), world_to_projection);
	output_mgr->bindEndGBuffer(device_context);
	profiler.endTimestamp("GBuffer");


	//----------------------------------------------------------------------------------
	// Deferred Render (opaque objects)
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Deferred");
	output_mgr->bindBeginDeferred(device_context);
	deferred_pass->render(settings.getBRDF());
	output_mgr->bindEndDeferred(device_context);
	profiler.endTimestamp("Deferred");


	output_mgr->bindBeginForward(device_context);

	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Skybox");
	sky_pass->render(skybox);
	profiler.endTimestamp("Skybox");

	
	//----------------------------------------------------------------------------------
	// Forward Render (overrided shaders and transparent objects)
	//----------------------------------------------------------------------------------
	profiler.beginTimestamp("Forward");

	profiler.beginTimestamp("Opaque");
	forward_pass->renderOverrided(scene.getECS(), world_to_projection, skybox);
	profiler.endTimestamp("Opaque");

	profiler.beginTimestamp("Transparent");
	forward_pass->renderTransparent(scene.getECS(), world_to_projection, skybox, settings.getBRDF());
	profiler.endTimestamp("Transparent");

	profiler.endTimestamp("Forward");

	output_mgr->bindEndForward(device_context);
}


template<typename CameraT>
void XM_CALLCONV Renderer::renderFalseColor(Scene& scene,
                                            CameraT& camera,
                                            FXMMATRIX world_to_projection) {
	profiler.beginTimestamp("Forward");
	output_mgr->bindBeginForward(device_context);

	const auto& settings = camera.getSettings();
	forward_pass->renderFalseColor(scene.getECS(), world_to_projection, settings.getFalseColorMode());

	output_mgr->bindEndForward(device_context);
	profiler.endTimestamp("Forward");
}

} //namespace render
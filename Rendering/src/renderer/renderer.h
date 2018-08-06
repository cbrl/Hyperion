#pragma once

#include "directx/d3d11.h"
#include "hlsl.h"
#include "renderer/output_mgr/output_mgr.h"
#include "renderer/render_state_mgr/render_state_mgr.h"

#include "scene/scene.h"
#include "renderer/pass/light/light_pass.h"
#include "renderer/pass/forward/forward_pass.h"
#include "renderer/pass/sky/sky_pass.h"
#include "renderer/pass/bounding_volume/bounding_volume_pass.h"
#include "renderer/pass/text/text_pass.h"
#include "ui/user_interface.h"


class RenderingMgr;


class Renderer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Renderer(DisplayConfig& display_config,
	         ID3D11Device& device,
	         ID3D11DeviceContext& device_context,
	         SwapChain& swap_chain,
	         ResourceMgr& resource_mgr);

	Renderer(const Renderer& renderer) = delete;
	Renderer(Renderer&& renderer) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Renderer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Renderer& operator=(const Renderer& renderer) = delete;
	Renderer& operator=(Renderer&& renderer) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void onResize() {
		output_mgr->resizeBuffers();
	}

	void render(Scene& scene);


private:
	template<typename CameraT>
	void renderCamera(Scene& scene, const CameraT& camera);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::reference_wrapper<ID3D11Device>        device;
	std::reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<OutputMgr> output_mgr;
	unique_ptr<RenderStateMgr> render_state_mgr;

	// Renderers
	unique_ptr<LightPass>          light_pass;
	unique_ptr<ForwardPass>        forward_pass;
	unique_ptr<SkyPass>            sky_pass;
	unique_ptr<BoundingVolumePass> bounding_volume_pass;
	unique_ptr<TextPass>           text_pass;
	unique_ptr<UserInterface>      ui;
};


#include "renderer.tpp"

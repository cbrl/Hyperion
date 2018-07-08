#pragma once

#include "directx/d3d11.h"
#include "hlsl.h"
#include "render_state_mgr.h"

#include "renderer/pass/light/light_pass.h"
#include "renderer/pass/forward/forward_pass.h"
#include "renderer/pass/sky/sky_pass.h"
#include "renderer/pass/bounding_volume/bounding_volume_pass.h"
#include "renderer/pass/text/text_pass.h"
#include "ui/user_interface.h"

class Engine;

class Renderer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Renderer(ID3D11Device& device, ID3D11DeviceContext& device_context);
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

	void render(const Engine& engine);


private:
	template<typename CameraT>
	void renderCamera(const Engine& engine, const CameraT& camera);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	reference_wrapper<ID3D11Device>        device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	// Renderers
	unique_ptr<LightPass>          light_pass;
	unique_ptr<ForwardPass>        forward_pass;
	unique_ptr<SkyPass>            sky_pass;
	unique_ptr<BoundingVolumePass> bounding_volume_pass;
	unique_ptr<TextPass>           text_pass;
	unique_ptr<UserInterface>      ui;
};


#include "renderer.tpp"

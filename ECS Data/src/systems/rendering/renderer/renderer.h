#pragma once

#include <d3d11.h>
#include <DirectXColors.h>

#include "system/system.h"
#include "util/engine_util.h"
#include "hlsl.h"
#include "pipeline.h"
#include "render_state_mgr.h"
#include "systems/rendering/renderer/pass/light/light_pass.h"
#include "systems/rendering/renderer/pass/forward/forward_pass.h"
#include "systems/rendering/renderer/pass/sky/sky_pass.h"
#include "systems/rendering/renderer/pass/text/text_pass.h"

class Engine;


class Renderer final : public System<Renderer> {
public:
	Renderer(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~Renderer() = default;

	void update(const Engine& engine) override;

	template<typename CameraT>
	void renderCamera(const Engine& engine, const CameraT& camera);


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	// Renderers
	unique_ptr<LightPass> light_pass;
	unique_ptr<ForwardPass> forward_pass;
	unique_ptr<SkyPass> sky_pass;
	unique_ptr<TextPass> text_pass;
};


#include "renderer.tpp"

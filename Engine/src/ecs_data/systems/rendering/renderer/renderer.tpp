template<typename CameraT>
void Renderer::RenderCamera(const Engine& engine, const CameraT& camera) {

	// Get necessary objects
	auto& ecs_engine = engine.GetECS();
	const auto& rendering_mgr = engine.GetRenderingMgr();
	const auto& render_state_mgr = rendering_mgr.GetRenderStateMgr();


	// Camera variables
	const auto transform = ecs_engine.getComponent<CameraTransform>(camera.getOwner());
	const auto world_to_camera = transform->GetWorldToObjectMatrix();
	const auto camera_to_projection = camera.GetProjectionMatrix();
	const auto world_to_projection = world_to_camera * camera_to_projection;


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------

	light_pass->Render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------

	camera.BindViewport(device_context);
	rendering_mgr.BindDefaultRenderTarget();

	forward_pass->BindDefaultRenderStates(render_state_mgr);
	forward_pass->Render(ecs_engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------

	const auto skybox = ecs_engine.getComponent<SkyBox>(camera.getOwner());
	if (skybox) {
		sky_pass->BindDefaultRenderStates(render_state_mgr);
		sky_pass->Render(*skybox);
	}
}

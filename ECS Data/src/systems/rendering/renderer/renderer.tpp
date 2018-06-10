template<typename CameraT>
void Renderer::renderCamera(const Engine& engine, const CameraT& camera) {

	// Get necessary objects
	auto& ecs_engine             = engine.getECS();
	const auto& rendering_mgr    = engine.getRenderingMgr();
	const auto& render_state_mgr = rendering_mgr.getRenderStateMgr();


	// Camera variables
	const auto transform            = ecs_engine.getComponent<CameraTransform>(camera.getOwner());
	const auto world_to_camera      = transform->getWorldToObjectMatrix();
	const auto camera_to_projection = camera.getProjectionMatrix();
	const auto world_to_projection  = world_to_camera * camera_to_projection;


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------

	light_pass->render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------

	camera.bindViewport(device_context);
	rendering_mgr.bindDefaultRenderTarget();

	forward_pass->bindDefaultRenderStates(render_state_mgr);
	forward_pass->render(ecs_engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------

	const auto skybox = ecs_engine.getComponent<SkyBox>(camera.getOwner());
	if (skybox) {
		sky_pass->bindDefaultRenderStates(render_state_mgr);
		sky_pass->render(*skybox);
	}
}

template<typename CameraT>
void Renderer::renderCamera(const Engine& engine, const CameraT& camera) {

	// Get the ECS and rendering manager
	auto& ecs_engine          = engine.getECS();
	const auto& rendering_mgr = engine.getRenderingMgr();


	// Camera variables
	const auto* transform = ecs_engine.getComponent<Transform>(camera.getOwner());
	assert(transform != nullptr);

	const auto world_to_camera      = transform->getWorldToObjectMatrix();
	const auto camera_to_projection = camera.getCameraToProjectionMatrix();
	const auto world_to_projection  = world_to_camera * camera_to_projection;
	const auto* skybox              = camera.getSkybox();


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------
	light_pass->render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Bind the camera's viewport and the rebind default render target
	// (The light pass has to bind a different one)
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);
	rendering_mgr.bindDefaultRenderTarget();


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	forward_pass->render(engine, skybox, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render bounding volumes
	//----------------------------------------------------------------------------------
	bounding_volume_pass->render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	sky_pass->render(engine, skybox);
}

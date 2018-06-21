template<typename CameraT>
void Renderer::renderCamera(const Engine& engine, const CameraT& camera) {

	// Get the ECS and rendering manager
	auto& ecs_engine          = engine.getECS();
	const auto& rendering_mgr = engine.getRenderingMgr();


	// Camera variables
	const auto transform            = ecs_engine.getComponent<CameraTransform>(camera.getOwner());
	const auto world_to_camera      = transform->getWorldToObjectMatrix();
	const auto camera_to_projection = camera.getProjectionMatrix();
	const auto world_to_projection  = world_to_camera * camera_to_projection;
	const auto skybox               = ecs_engine.getComponent<SkyBox>(camera.getOwner());


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------
	light_pass->render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Rebind the camera's viewport and the default render target
	// (The light pass has to bind different ones)
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);
	rendering_mgr.bindDefaultRenderTarget();


	//----------------------------------------------------------------------------------
	// Render objects with forward shaderaaa
	//----------------------------------------------------------------------------------
	forward_pass->render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render bounding volumes
	//----------------------------------------------------------------------------------
	bounding_volume_pass->render(engine, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	if (skybox) {
		sky_pass->render(engine, *skybox);
	}
}

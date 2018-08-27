template<typename CameraT>
void Renderer::renderCamera(Scene& scene, const CameraT& camera) {

	// Get the ECS
	auto& ecs_engine = scene.getECS();

	// Camera variables
	const auto& settings  = camera.getSettings();
	const auto* transform = camera.getOwner()->getComponent<Transform>();
	assert(transform != nullptr);

	const auto world_to_camera      = transform->getWorldToObjectMatrix();
	const auto camera_to_projection = camera.getCameraToProjectionMatrix();
	const auto world_to_projection  = world_to_camera * camera_to_projection;
	const auto* skybox              = camera.getSettings().getSkybox();


	//----------------------------------------------------------------------------------
	// Process the light buffers
	//----------------------------------------------------------------------------------
	light_pass->render(scene, world_to_projection);


	//----------------------------------------------------------------------------------
	// Bind the camera's viewport and the forward output state
	//----------------------------------------------------------------------------------
	camera.bindViewport(device_context);
	output_mgr->bindBeginForward(device_context);


	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	switch (settings.getLightingMode()) {
		case LightingMode::Default:
			forward_pass->renderOpaque(scene, world_to_projection, skybox);
			forward_pass->renderTransparent(scene, world_to_projection, skybox);
			break;

		case LightingMode::Unlit:
			forward_pass->renderUnlit(scene, world_to_projection, skybox);
			break;

		case LightingMode::FalseColorPosition:
			forward_pass->renderFalseColor(scene, world_to_projection, FalseColor::Position);
			break;

		case LightingMode::FalseColorNormal:
			forward_pass->renderFalseColor(scene, world_to_projection, FalseColor::Normal);
			break;

		case LightingMode::FalseColorDepth:
			forward_pass->renderFalseColor(scene, world_to_projection, FalseColor::Depth);
			break;

		default:
			break;
	}

	if (settings.hasRenderOption(RenderOptions::Wireframe))
		forward_pass->renderWireframe(scene, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render bounding volumes
	//----------------------------------------------------------------------------------
	if (settings.hasRenderOption(RenderOptions::BoundingVolume))
		bounding_volume_pass->render(scene, world_to_projection);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	sky_pass->render(skybox);


	// Clear the bound forward state
	output_mgr->bindEndForward(device_context);
}

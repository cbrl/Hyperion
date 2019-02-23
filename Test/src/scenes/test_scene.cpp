#include "test_scene.h"
#include "engine/engine.h"


TestScene::TestScene()
    : Scene("Test Scene") {
}


void TestScene::initialize(Engine& engine) {
	
	auto& input          = engine.getInput();
	auto& rendering_mgr  = engine.getRenderingMgr();
	auto& device         = rendering_mgr.getDevice();
	auto& device_context = rendering_mgr.getDeviceContext();
	auto& resource_mgr   = rendering_mgr.getResourceMgr();

	//----------------------------------------------------------------------------------
	// Add systems
	//----------------------------------------------------------------------------------

	// User Interface
	addSystem<UserInterface>(engine);

	// Camera motor system: moves an entity with a camera and camera movement component (entity requires CameraMovement component)
	addSystem<CameraMotorSystem>(input);

	// Mouse rotation system: reads mouse input to rotate an entity (entity requires MouseRotation component)
	addSystem<MouseRotationSystem>(input);

	// Axis rotation system: rotates an entity on an axis (entity requires AxisRotation component)
	addSystem<AxisRotationSystem>();

	// Axis orbit system: orbits an entity around an axis (entity requires AxisOrbit component)
	addSystem<AxisOrbitSystem>();


	//----------------------------------------------------------------------------------
	// Camera
	//----------------------------------------------------------------------------------

	// Create the camera
	EntityPtr camera = addEntity<PlayerCamera>(device, engine.getWindow().getClientSize());
	camera->setName("Perspective Camera");
	camera->addComponent<AmbientLight>().setColor(f32_4{0.16f, 0.16f, 0.16f, 1.0f});

	// Set the parameters
	auto* cam = camera->getComponent<PerspectiveCamera>();

	// View settings
	cam->getViewport().setDepth(0.0f, 1.0f);
	cam->setZDepth(0.01f, 1000.0f);
	cam->setFOV(XM_PI / 3.0f);

	// Skybox
	cam->getSettings().setSkybox(resource_mgr.getOrCreate<Texture>(L"../data/Textures/grasscube1024.dds"));

	// Fog
	auto& fog = cam->getSettings().getFog();
	fog.density = 0.05f;
	fog.color = {0.2f, 0.2f, 0.2f};

	// Initial position
	camera->getComponent<Transform>()->setPosition(f32_3{0.0f, 3.0f, -3.0f});

	// Mouse control settings
	camera->getComponent<MouseRotation>()->setSensitivity(0.01f);


	//----------------------------------------------------------------------------------
	// Models
	//----------------------------------------------------------------------------------

	// Model config
	ModelConfig<VertexPositionNormalTexture> config;
	config.flip_winding = false;
	config.flip_uv      = false;

	// Inverted model config
	ModelConfig<VertexPositionNormalTexture> inv_config;
	config.flip_winding = true;
	config.flip_uv      = false;

	// Create blueprints
	//auto main_bp          = resource_mgr.getOrCreate<ModelBlueprint>(L"../data/models/test/test.obj", config);
	auto sphere_bp        = BlueprintFactory::CreateSphere(resource_mgr, config, 1.0f);
	auto cube_bp          = BlueprintFactory::CreateCube(resource_mgr, config, 2.0f);
	auto inverted_cube_bp = BlueprintFactory::CreateCube(resource_mgr, inv_config, 10.0f);
	auto cylinder_bp      = BlueprintFactory::CreateCylinder(resource_mgr, config, 2.0f, 2.0f);

	// Bounding Cube
	{
		EntityPtr inv_cube = importModel(device, inverted_cube_bp);
		inv_cube->setName("Bounding Cube");
		inv_cube->getComponent<Transform>()->setPosition(f32_3{0.0f, 5.0f, 0.0f});

		auto& mat = inv_cube->getComponent<Model>()->getMaterial();
		mat.params.base_color = {0.2f, 0.2f, 0.8f, 1.0f};
		mat.params.roughness  = 0.8f;
		mat.params.metalness  = 0.1f;
	}

	// Cube
	{
		EntityPtr cube = importModel(device, cube_bp);
		cube->setName("Cube");
		cube->getComponent<Transform>()->setPosition(f32_3{-2.5f, 1.0f, 1.5f});

		auto& mat = cube->getComponent<Model>()->getMaterial();
		mat.params.base_color = {1.0f, 0.0f, 0.0f, 1.0f};
		mat.params.roughness  = 0.3f;
		mat.params.metalness  = 0.65f;
	}

	// Cylinder
	{
		EntityPtr cylinder = importModel(device, cylinder_bp);
		cylinder->setName("Cylinder");
		cylinder->getComponent<Transform>()->setPosition(f32_3{2.5f, 1.0f, 0.75f});

		auto& mat = cylinder->getComponent<Model>()->getMaterial();
		mat.params.base_color = {0.0f, 1.0f, 0.0f, 1.0f};
		mat.params.roughness  = 0.7f;
		mat.params.metalness  = 0.6f;
	}


	// Sphere
	{
		EntityPtr sphere = importModel(device, sphere_bp);
		sphere->setName("Sphere");
		sphere->getComponent<Transform>()->setPosition(f32_3{0.0f, 1.0f, -1.0f});
		sphere->getComponent<Transform>()->setScale(f32_3{2.0f});
		auto& mat = sphere->getComponent<Model>()->getMaterial();
		mat.params.base_color = {1.0f, 0.7f, 0.0f, 1.0f};
		mat.params.roughness  = 0.5f;
		mat.params.metalness  = 0.3f;
	}


	// Sphere Light
	//EntityPtr sphere_light = importModel(device, sphere_bp);
	//sphere_light->setName("Sphere Light");
	//sphere_light->addComponent<PointLight>();
	//sphere_light->getComponent<Transform>()->setPosition(f32_3{0.0f, 4.0f, 0.0f});

	//auto* rotation = sphere_light->addComponent<AxisRotation>();
	//rotation->setAxis(AxisRotation::Axis::Y);
	//rotation->setSpeedY(1.5f);

	//auto* orbit = sphere_light->addComponent<AxisOrbit>();
	//orbit->setSpeed(0.5f);


	//----------------------------------------------------------------------------------
	// Lights
	//----------------------------------------------------------------------------------

	// Sphere light
	{
		//auto* light = sphere_light->getComponent<PointLight>();
		auto entity = addEntity();
		entity->setName("Point Light");
		entity->getComponent<Transform>()->setPosition(f32_3{0.0f, 4.0f, 0.0f});

		auto& light = entity->addComponent<PointLight>();
		light.setBaseColor(f32_3{1.0f, 1.0f, 1.0f});
		light.setIntensity(9.0f);
		light.setAttenuation(f32_3{0.0f, 0.1f, 0.1f});
		light.setRange(100.0f);
		light.setShadows(true);
	}

	// Camera light
	{
		const auto light = addEntity();
		light->setName("Camera Light");
		camera->addChild(light);

		auto& spot_light = light->addComponent<SpotLight>();
		spot_light.setBaseColor(f32_3{0.9f, 0.9f, 0.9f});
		spot_light.setIntensity(7.0f);
		spot_light.setAttenuation(f32_3{0.0f, 0.1f, 0.1f});
		spot_light.setRange(100.0f);
		spot_light.setUmbraAngle(XM_PI / 6.0f);
		spot_light.setPenumbraAngle(XM_PI / 4.0f);
		spot_light.setShadows(true);

		auto* transform = light->getComponent<Transform>();
		transform->setPosition(f32_3{-1.0f, 0.0f, 0.0f});
	}

	// Directional Light
	{
		//const auto dir_light = addEntity();
		//auto* light = ecs->addComponent<DirectionalLight>(dir_light);
		//light->setDiffuseColor(f32_4{ 0.0f, 0.0f, 1.0f, 0.0f });
		//light->setRange(100.0f);
		//light->setSize(f32_2{ 30.0f, 30.0f });

		//auto transform = ecs_engine.getComponent<Transform>(dir_light);
		//transform->setPosition(f32_3{ 0.0f, 4.0f, 0.0f });
		//transform->setRotation(f32_3{ XM_PIDIV4, 0.0f, 0.0f });
	}


	//----------------------------------------------------------------------------------
	// Text objects
	//----------------------------------------------------------------------------------

	auto font = resource_mgr.getOrCreate<Font>(L"../data/fonts/courier-12.spritefont");

	scene_name_text = addEntity();
	scene_name_text->setName("Scene Name Text");
	scene_name_text->addComponent<Text>(font);
	scene_name_text->getComponent<Text>()->setText(StrToWstr(this->getName()));
	scene_name_text->getComponent<Transform>()->setPosition(f32_3{10, 10, 0});
}


void TestScene::update(Engine& engine) {

}

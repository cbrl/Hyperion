#include "test_scene.h"
#include "engine/engine.h"


TestScene::TestScene() : Scene("Test Scene") {
}


void TestScene::initialize(const Engine& engine) {

	const auto& rendering_mgr = engine.getRenderingMgr();
	auto& device              = rendering_mgr.getDevice();
	auto& device_context      = rendering_mgr.getDeviceContext();
	auto& resource_mgr        = rendering_mgr.getResourceMgr();

	//----------------------------------------------------------------------------------
	// Add systems
	//----------------------------------------------------------------------------------

	// User Interface
	addSystem<UserInterface>();

	// Camera motor system: moves an entity with a camera and camera movement component (entity requires CameraMovement component)
	addSystem<CameraMotorSystem>();

	// Mouse rotation system: reads mouse input to rotate an entity (entity requires MouseRotation component)
	addSystem<MouseRotationSystem>();

	// Axis rotation system: rotates an entity on an axis (entity requires AxisRotation component)
	addSystem<AxisRotationSystem>();

	// Axis orbit system: orbits an entity around an axis (entity requires AxisOrbit component)
	addSystem<AxisOrbitSystem>();


	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	// Create the camera
	EntityPtr camera = addEntity<PlayerCamera>(device, engine.getWindow().getClientSize());
	camera->setName("Perspective Camera");
	camera->addComponent<AmbientLight>()->setColor(vec4_f32{ 0.16f, 0.16f, 0.16f, 1.0f });

	// Set the parameters
	auto* cam = camera->getComponent<PerspectiveCamera>();

	cam->getViewport().setDepth(0.0f, 1.0f);
	cam->setZDepth(0.01f, 1000.0f);
	cam->setFOV(XM_PI / 3.0f);
	cam->getSettings().setSkybox( resource_mgr.getOrCreate<Texture>(L"../data/Textures/grasscube1024.dds") );

	auto& fog = cam->getSettings().getFog();
	fog.density = 0.05f;
	fog.color   = { 0.2f, 0.2f, 0.2f };

	camera->getComponent<Transform>()->setPosition(vec3_f32{ 0.0f, 3.0f, -3.0f });
	camera->getComponent<MouseRotation>()->setSensitivity(0.01f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	// Model config
	ModelConfig<VertexPositionNormalTexture> config;
	config.flip_winding = false;
	config.flip_uv      = false;

	ModelConfig<VertexPositionNormalTexture> inv_config;
	config.flip_winding = true;
	config.flip_uv      = false;

	auto main_bp          = resource_mgr.getOrCreate<ModelBlueprint>(L"../data/models/test/test.obj", config);
	auto sphere_bp        = BlueprintFactory::CreateSphere(resource_mgr, config, 1.0f);
	auto cube_bp          = BlueprintFactory::CreateCube(resource_mgr, config, 2.0f);
	auto inverted_cube_bp = BlueprintFactory::CreateCube(resource_mgr, inv_config, 10.0f);
	auto cylinder_bp      = BlueprintFactory::CreateCylinder(resource_mgr, config, 2.0f, 2.0f);


	// Scene model
	//EntityPtr test_model = addEntity();
	//test_model->setName("Test Model");
	//test_model->addComponent<ModelRoot>(device, main_bp);

	// Bounding Cube
	EntityPtr inv_cube = addEntity<ModelT>(device, inverted_cube_bp);
	inv_cube->setName("Bounding Cube");
	inv_cube->getComponent<Transform>()->setPosition(vec3_f32{0.0f, 5.0f, 0.0f});
	inv_cube->getComponent<ModelRoot>()->forEachModel([](Model& model) {
		model.getMaterial().params.base_color = {0.2f, 0.2f, 0.8f, 1.0f};
		model.getMaterial().params.roughness  = 0.8f;
		model.getMaterial().params.metalness  = 0.1f;
	});

	// Cube
	EntityPtr cube = addEntity<ModelT>(device, cube_bp);
	cube->setName("Cube");
	cube->getComponent<Transform>()->setPosition(vec3_f32{-2.5f, 1.0f, 1.5f});
	cube->getComponent<ModelRoot>()->forEachModel([](Model& model) {
		model.getMaterial().params.base_color = {1.0f, 0.0f, 0.0f, 1.0f};
		model.getMaterial().params.roughness  = 0.3f;
		model.getMaterial().params.metalness  = 0.65f;
	});

	// Cylinder
	EntityPtr cylinder = addEntity<ModelT>(device, cylinder_bp);
	cylinder->setName("Cylinder");
	cylinder->getComponent<Transform>()->setPosition(vec3_f32{2.5f, 1.0f, 0.75f});
	cylinder->getComponent<ModelRoot>()->forEachModel([](Model& model) {
		model.getMaterial().params.base_color = {0.0f, 1.0f, 0.0f, 1.0f};
		model.getMaterial().params.roughness  = 0.7f;
		model.getMaterial().params.metalness  = 0.6f;
	});


	// Sphere
	EntityPtr sphere = addEntity<ModelT>(device, sphere_bp);
	sphere->setName("Sphere");
	sphere->getComponent<Transform>()->setPosition(vec3_f32{0.0f, 1.0f, -1.0f});
	sphere->getComponent<Transform>()->setScale(vec3_f32{2.0f});
	sphere->getComponent<ModelRoot>()->forEachModel([](Model& model) {
		model.getMaterial().params.base_color = {1.0f, 0.7f, 0.0f, 1.0f};
		model.getMaterial().params.roughness = 0.5f;
		model.getMaterial().params.metalness = 0.3f;
	});


	// Sphere Light
	//EntityPtr sphere_light = addEntity<ModelT>(device, sphere_bp);
	//sphere_light->setName("Sphere Light");
	//sphere_light->addComponent<PointLight>();
	//sphere_light->getComponent<Transform>()->setPosition(vec3_f32{0.0f, 4.0f, 0.0f});

	//auto* rotation = sphere_light->addComponent<AxisRotation>();
	//rotation->setAxis(AxisRotation::Axis::Y);
	//rotation->setSpeedY(1.5f);

	//auto* orbit = sphere_light->addComponent<AxisOrbit>();
	//orbit->setSpeed(0.5f);


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------
	
	// Sphere light
	{
		//auto* light = sphere_light->getComponent<PointLight>();
		auto entity = addEntity();
		entity->setName("Point Light");
		entity->getComponent<Transform>()->setPosition(vec3_f32{0.0f, 4.0f, 0.0f});

		auto* light = entity->addComponent<PointLight>();
		light->setBaseColor(vec3_f32{ 1.0f, 1.0f, 1.0f });
		light->setIntensity(9.0f);
		light->setAttenuation(vec3_f32{ 0.0f, 0.1f, 0.1f });
		light->setRange(100.0f);
		light->setShadows(true);
	}

	// Camera light
	{
		const auto light = addEntity();
		light->setName("Camera Light");
		camera->addChild(light);

		auto* spot_light = light->addComponent<SpotLight>();
		spot_light->setBaseColor(vec3_f32{ 0.9f, 0.9f, 0.9f });
		spot_light->setIntensity(7.0f);
		spot_light->setAttenuation(vec3_f32{0.0f, 0.1f, 0.1f});
		spot_light->setRange(100.0f);
		spot_light->setUmbraAngle(XM_PI / 6.0f);
		spot_light->setPenumbraAngle(XM_PI / 4.0f);
		spot_light->setShadows(true);

		auto* transform = light->getComponent<Transform>();
		transform->setPosition(vec3_f32{ -1.0f, 0.0f, 0.0f });
	}

	// Directional Light
	{
		//const auto dir_light = addEntity();
		//auto* light = ecs->addComponent<DirectionalLight>(dir_light);
		//light->setDiffuseColor(vec4_f32{ 0.0f, 0.0f, 1.0f, 0.0f });
		//light->setRange(100.0f);
		//light->setSize(vec2_f32{ 30.0f, 30.0f });

		//auto transform = ecs_engine.getComponent<Transform>(dir_light);
		//transform->setPosition(vec3_f32{ 0.0f, 4.0f, 0.0f });
		//transform->setRotation(vec3_f32{ XM_PIDIV4, 0.0f, 0.0f });
	}


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------

	auto font = resource_mgr.getOrCreate<Font>(L"../data/fonts/courier-12.spritefont");

	// CPU Usage
	text_cpu = addEntity<>();
	text_cpu->setName("CPU Usage");
	text_cpu->addComponent<Text>(font);
	text_cpu->getComponent<Transform>()->setPosition(vec3_f32{ 10, 10, 0 });

	// RAM Usage
	text_ram = addEntity<>();
	text_ram->setName("Ram Usage");
	text_ram->addComponent<Text>(font);
	text_ram->getComponent<Transform>()->setPosition(vec3_f32{ 10, 80, 0 });

	// Mouse Position
	text_mouse = addEntity<>();
	text_mouse->setName("Mouse Position");
	text_mouse->addComponent<Text>(font);
	text_mouse->getComponent<Transform>()->setPosition(vec3_f32{ 10, 150, 0 });
}


void TestScene::update(Engine& engine) {

	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	const vec2_i32 mouse_pos  = engine.getInput().getMousePosition();
	const f64 total_cpu_usage = engine.getSysMon().cpu().getTotalCpuPercentage();
	const f64 proc_cpu_usage  = engine.getSysMon().cpu().getProcessCpuPercentage();
	const u64 total_mem_usage = engine.getSysMon().memory().getTotalUsedPhysicalMem();
	const u64 proc_mem_usage  = engine.getSysMon().memory().getProcessUsedPhysicalMem();

	static std::wostringstream cpu_str;
	static std::wostringstream mem_str;

	// Build the CPU usage string
	cpu_str.clear();
	cpu_str.str(L"");
	cpu_str.precision(4);
	cpu_str << L"CPU Usage\nTotal: " << total_cpu_usage << L"%"
	        << L"\nProcess: "        << proc_cpu_usage  << L"%";

	// Build the memory usage string
	mem_str.clear();
	mem_str.str(L"");
	mem_str.precision(4);
	mem_str << L"RAM Usage\nTotal: " << static_cast<f64>(total_mem_usage) / 1e6 << L"MB"
	        << L"\nProcess: "        << static_cast<f64>(proc_mem_usage) / 1e6  << L"MB";

	// CPU Usage
	if (text_cpu.valid()) {
		if (auto* text = text_cpu->getComponent<Text>())
			text->setText(cpu_str.str());
	}

	// RAM Usage
	if (text_ram.valid()) {
		if (auto* text = text_ram->getComponent<Text>())
			text->setText(mem_str.str());
	}

	// Mouse Activity
	if (text_mouse.valid()) {
		if (auto* text = text_mouse->getComponent<Text>())
			text->setText(L"Mouse \nX: " + std::to_wstring(mouse_pos[0])
			              + L"\nY: "     + std::to_wstring(mouse_pos[1]));
	}
}

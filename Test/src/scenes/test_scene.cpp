#include "stdafx.h"
#include "test_scene.h"
#include "engine/engine.h"


TestScene::TestScene() : Scene("Test Scene") {
}


void TestScene::load(const Engine& engine) {

	const auto& rendering_mgr = engine.getRenderingMgr();
	auto& device              = rendering_mgr.getDevice();
	auto& device_context      = rendering_mgr.getDeviceContext();
	auto& resource_mgr        = rendering_mgr.getResourceMgr();


	//----------------------------------------------------------------------------------
	// Add systems
	//----------------------------------------------------------------------------------

	// Transform system: updates transforms when they're modified
	ecs->addSystem<TransformSystem>();

	// Camera system: updates camera buffers
	ecs->addSystem<CameraSystem>();

	// Model system: updates model buffers
	ecs->addSystem<ModelSystem>();

	// Camera motor system: moves an entity with a camera and camera movement component
	ecs->addSystem<CameraMotorSystem>();

	// Mouse rotation system: uses mouse input to rotate an entity's transform
	ecs->addSystem<MouseRotationSystem>();

	ecs->addSystem<AxisRotationSystem>();
	ecs->addSystem<AxisOrbitSystem>();


	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	// Create the camera
	const handle64 camera = addEntity<PlayerCamera>(device, engine.getWindowSize());

	// Set the parameters
	auto cam = ecs->getComponent<PerspectiveCamera>(camera);

	cam->getViewport().setDepth(0.0f, 1.0f);
	cam->setZDepth(0.01f, 1000.0f);
	cam->setFOV(XM_PI / 3.0f);
	cam->getSettings().setSkybox(
		resource_mgr.getOrCreate<Texture>(L"../data/Textures/grasscube1024.dds"));

	auto& fog = cam->getSettings().getFog();
	fog.color = vec4_f32{0.2f, 0.2f, 0.2f, 1.0f};
	fog.start = 150.0f;
	fog.range = 100.0f;

	ecs->getComponent<Transform>(camera)->setPosition(vec3_f32{0.0f, 6.0f, -2.0f});
	ecs->getComponent<MouseRotation>(camera)->setSensitivity(0.01f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	// Scene model
	auto bp = resource_mgr.getOrCreate<ModelBlueprint>(L"../data/models/test/test.obj");
	addEntity<BasicModel>(device, bp);

	// Sphere
	auto sphere_bp = BlueprintFactory::CreateSphere<VertexPositionNormalTexture>(resource_mgr, 1.0f);
	const handle64 sphere = addEntity<BasicModel>(device, sphere_bp);

	ecs->getComponent<Transform>(sphere)->setPosition(vec3_f32{ 3.0f, 2.0f, 0.0f });

	auto rotation = ecs->addComponent<AxisRotation>(sphere);
	rotation->setAxis(AxisRotation::Axis::Y);
	rotation->setSpeedY(1.5f);

	auto orbit = ecs->addComponent<AxisOrbit>(sphere);
	orbit->setSpeed(0.5f);


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------
	
	// Sphere light
	{
		auto light = ecs->addComponent<SpotLight>(sphere);
		light->setAmbientColor(vec4_f32(0.15f, 0.15f, 0.15f, 1.0f));
		light->setDiffuseColor(vec4_f32(1.0f, 0.9f, 0.5f, 1.0f));
		light->setAttenuation(vec3_f32(0.1f, 0.15f, 0.0f));
		light->setSpecular(vec4_f32(1.0f, 1.0f, 1.0f, 1.0f));
		light->setRange(100.0f);
		light->setUmbraAngle(XM_PI / 6.0f);
		light->setPenumbraAngle(XM_PI / 3.0f);
		light->setShadows(true);
	}

	// Camera light
	{
		const auto spot_light = addEntity<BasicSpotLight>(); 
		auto light = ecs->getComponent<SpotLight>(spot_light);
		light->setAmbientColor(vec4_f32(0.15f, 0.15f, 0.15f, 1.0f));
		light->setDiffuseColor(vec4_f32(0.8f, 0.8f, 1.0f, 1.0f));
		light->setAttenuation(vec3_f32(0.05f, 0.2f, 0.0f));
		light->setSpecular(vec4_f32(1.0f, 1.0f, 1.0f, 1.0f));
		light->setRange(100.0f);
		light->setUmbraAngle(XM_PI / 6.0f);
		light->setPenumbraAngle(XM_PI / 3.0f);
		light->setShadows(true);

		auto transform = ecs->getComponent<Transform>(spot_light);
		transform->setPosition(vec3_f32(-1.0f, 0.0f, 0.0f));
		transform->setParent(camera);
	}

	// Directional Light
	{
		//const auto dir_light = addEntity<BasicDirectionalLight>(ecs_engine);
		//auto light = ecs_engine.getComponent<DirectionalLight>(dir_light);
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

	const wstring font(L"../data/fonts/courier-12.spritefont");

	texts.try_emplace("FPS", resource_mgr, font);
	texts.at("FPS").setPosition(vec2_f32(10, 10));

	texts.try_emplace("FrameTime", resource_mgr, font);
	texts.at("FrameTime").setPosition(vec2_f32(10, 40));

	texts.try_emplace("CPU", resource_mgr, font);
	texts.at("CPU").setPosition(vec2_f32(10, 70));

	texts.try_emplace("RAM", resource_mgr, font);
	texts.at("RAM").setPosition(vec2_f32(10, 140));

	texts.try_emplace("Mouse", resource_mgr, font);
	texts.at("Mouse").setPosition(vec2_f32(10, 210));
}


void TestScene::tick(const Engine& engine) {

	// Update the active systems in the ECS
	ecs->update(engine);


	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	const vec2_i32 mouse_delta = engine.getInput().getMouseDelta();
	const u32 fps              = engine.getFPSCounter().getFPS();
	const f64 delta_time       = engine.getTimer().deltaTime();
	const f64 total_cpu_usage  = engine.getSysMon().cpu().getTotalCpuPercentage();
	const f64 proc_cpu_usage   = engine.getSysMon().cpu().getProcessCpuPercentage();
	const u64 total_mem_usage  = engine.getSysMon().memory().getTotalUsedPhysicalMem();
	const u64 proc_mem_usage   = engine.getSysMon().memory().getProcessUsedPhysicalMem();

	static wostringstream cpu_str;
	static wostringstream mem_str;

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

	// FPS
	texts.at("FPS").setText(L"FPS: " + to_wstring(fps));

	// Frame Time
	texts.at("FrameTime").setText(L"Frame Time: " + to_wstring(delta_time * 1000.0) + L"ms");

	// CPU Usage
	texts.at("CPU").setText(cpu_str.str());

	// RAM Usage
	texts.at("RAM").setText(mem_str.str());

	// Mouse Activity
	texts.at("Mouse").setText(L"Mouse \nX: " + to_wstring(mouse_delta.x)
	                          + L"\nY: "     + to_wstring(mouse_delta.y));
}

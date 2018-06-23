#include "stdafx.h"
#include "test_scene.h"
#include "engine/engine.h"


static constexpr float z_near = 0.01f;
static constexpr float z_far  = 100.0f;
static constexpr float fov    = XM_PI / 3.0f;


TestScene::TestScene() : Scene("Test Scene") {
}


void TestScene::load(const Engine& engine) {

	auto& ecs_engine          = engine.getECS();
	const auto& rendering_mgr = engine.getRenderingMgr();
	auto& device              = rendering_mgr.getDevice();
	auto& device_context      = rendering_mgr.getDeviceContext();

	auto& resource_mgr = rendering_mgr.getResourceMgr();


	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	// Create the camera
	const handle64 camera = addEntity<PlayerCamera>(ecs_engine,
	                                                device,
	                                                device_context,
                                                    engine.getWindowWidth(),
	                                                engine.getWindowHeight());

	// Set the parameters
	auto cam = ecs_engine.getComponent<PerspectiveCamera>(camera);
	cam->setZDepth(z_near, z_far);
	cam->setFOV(fov);
	cam->setFog(Fog(f32_4(0.2f, 0.2f, 0.2f, 1.0f), 30.0f, 25.0f));
	cam->skybox().setTexture(resource_mgr.getOrCreate<Texture>(L"../data/Textures/grasscube1024.dds"));
	ecs_engine.getComponent<Transform>(camera)->setPosition(f32_3(0.0f, 4.0f, -2.0f));

	ecs_engine.getComponent<MouseRotation>(camera)->setSensitivity(0.01f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	// Scene model
	auto bp = resource_mgr.getOrCreate<ModelBlueprint>(L"../data/models/test/test.obj");
	addEntity<BasicModel>(ecs_engine, device, bp);

	// Sphere
	auto sphere_bp = BlueprintFactory::CreateSphere<VertexPositionNormalTexture>(resource_mgr, 1.0f);
	const handle64 sphere = addEntity<BasicModel>(ecs_engine, device, sphere_bp);

	auto rotation = ecs_engine.addComponent<AxisRotation>(sphere);
	rotation->setAxis(AxisRotation::Axis::Y);
	rotation->setSpeedY(0.5f);

	ecs_engine.getComponent<Transform>(sphere)->setPosition(f32_3{ 0.0f, 2.0f, 0.0f });
	//ecs_engine.getComponent<Transform>(sphere)->setPosition(f32_3(7.0f, 3.0f, 0.0f));


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------
	
	// Sphere light
	{
		auto light = ecs_engine.addComponent<SpotLight>(sphere);
		light->setAmbientColor(f32_4(0.15f, 0.15f, 0.15f, 1.0f));
		light->setDiffuseColor(f32_4(1.0f, 0.9f, 0.5f, 1.0f));
		light->setAttenuation(f32_3(0.1f, 0.15f, 0.0f));
		light->setSpecular(f32_4(1.0f, 1.0f, 1.0f, 1.0f));
		light->setRange(100.0f);
		light->setUmbraAngle(XM_PI / 6.0f);
		light->setPenumbraAngle(XM_PI / 3.0f);
		light->setShadows(true);
	}

	// Camera light
	{
		const auto spot_light = addEntity<BasicSpotLight>(ecs_engine);
		auto light = ecs_engine.getComponent<SpotLight>(spot_light);
		light->setAmbientColor(f32_4(0.15f, 0.15f, 0.15f, 1.0f));
		light->setDiffuseColor(f32_4(0.8f, 0.8f, 1.0f, 1.0f));
		light->setAttenuation(f32_3(0.05f, 0.2f, 0.0f));
		light->setSpecular(f32_4(1.0f, 1.0f, 1.0f, 1.0f));
		light->setRange(100.0f);
		light->setUmbraAngle(XM_PI / 6.0f);
		light->setPenumbraAngle(XM_PI / 3.0f);
		light->setShadows(true);

		auto transform = ecs_engine.getComponent<Transform>(spot_light);
		transform->setPosition(f32_3(-1.0f, 0.0f, 0.0f));
		//transform->SetRotation(f32_3(XM_PIDIV2, 0.0f, 0.0f));
		transform->setParent(camera);
	}

	// Directional Light
	{
		//const auto dir_light = addEntity<BasicDirectionalLight>(ecs_engine);
		//auto light = ecs_engine.getComponent<DirectionalLight>(dir_light);
		//light->setDiffuseColor(f32_4{ 0.0f, 0.0f, 1.0f, 0.0f });
		//light->setRange(100.0f);
		//light->setSize(f32_2{ 30.0f, 30.0f });

		//auto transform = ecs_engine.getComponent<Transform>(dir_light);
		//transform->setPosition(f32_3{ 0.0f, 4.0f, 0.0f });
		//transform->setRotation(f32_3{ XM_PIDIV4, 0.0f, 0.0f });
	}


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------

	const wstring font(L"../data/fonts/courier-12.spritefont");

	texts.try_emplace("FPS", resource_mgr, font);
	texts.at("FPS").setPosition(f32_2(10, 10));

	texts.try_emplace("FrameTime", resource_mgr, font);
	texts.at("FrameTime").setPosition(f32_2(10, 40));

	texts.try_emplace("CPU", resource_mgr, font);
	texts.at("CPU").setPosition(f32_2(10, 70));

	texts.try_emplace("RAM", resource_mgr, font);
	texts.at("RAM").setPosition(f32_2(10, 140));

	texts.try_emplace("Mouse", resource_mgr, font);
	texts.at("Mouse").setPosition(f32_2(10, 210));
}


void TestScene::tick(const Engine& engine) {

	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	const i32_2 mouse_delta       = engine.getInput().getMouseDelta();
	const u32 fps                = engine.getFPSCounter().getFPS();
	const float delta_time       = engine.getTimer().deltaTime();
	const double total_cpu_usage = engine.getSysMon().cpu().getTotalCpuPercentage();
	const double proc_cpu_usage  = engine.getSysMon().cpu().getProcessCpuPercentage();
	const u64 total_mem_usage    = engine.getSysMon().memory().getTotalUsedPhysicalMem();
	const u64 proc_mem_usage     = engine.getSysMon().memory().getProcessUsedPhysicalMem();

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
	mem_str << L"RAM Usage\nTotal: " << static_cast<double>(total_mem_usage) / 1e6 << L"MB"
	        << L"\nProcess: "        << static_cast<double>(proc_mem_usage) / 1e6  << L"MB";

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



	//----------------------------------------------------------------------------------
	// Move the sphere
	//----------------------------------------------------------------------------------

	// This could be made into a "rotate" system for use with the ECS.
	// Create a rotation component that contains various parameters (speed, axis, etc)
	//auto transform = engine.getECS().getComponent<Transform>(sphere);
	//const auto p = transform->getPosition();
	//const auto v = XMVector3Transform(p, XMMatrixRotationY(delta_time));
	//transform->setPosition(v);
}

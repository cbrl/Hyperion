#include "stdafx.h"
#include "test_scene.h"
#include "engine/engine.h"


static constexpr float z_near = 0.1f;
static constexpr float z_far  = 1000.0f;
static constexpr float fov    = XM_PI / 3.0f;


TestScene::TestScene(const Engine& engine)
	: Scene("Test Scene") {

	init(engine);
}


void TestScene::init(const Engine& engine) {

	auto& ecs_engine          = engine.getECS();
	const auto& rendering_mgr = engine.getRenderingMgr();
	auto& device              = rendering_mgr.getDevice();
	auto& device_context      = rendering_mgr.getDeviceContext();

	auto& resource_mgr = rendering_mgr.getResourceMgr();


	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	// Create the camera and skybox
	const handle64 camera = ecs_engine.createEntity<PlayerCamera>(device,
	                                                              device_context,
	                                                              engine.getWindowWidth(),
	                                                              engine.getWindowHeight(),
	                                                              resource_mgr.getOrCreate<Texture
	                                                              >(L"../data/Textures/grasscube1024.dds"));

	// Set the parameters
	auto cam = ecs_engine.getComponent<PerspectiveCamera>(camera);
	cam->setZDepth(z_near, z_far);
	cam->setFOV(fov);
	ecs_engine.getComponent<CameraTransform>(camera)->setPosition(float3(0.0f, 4.0f, -2.0f));

	entities.push_back(camera);


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------

	const auto point_light = ecs_engine.createEntity<BasicPointLight>();
	entities.push_back(point_light);

	const auto spot_light = ecs_engine.createEntity<BasicSpotLight>();
	entities.push_back(spot_light);

	// Point light
	{
		auto light = ecs_engine.getComponent<PointLight>(point_light);
		light->setAmbientColor(float4(0.15f, 0.15f, 0.15f, 1.0f));
		light->setDiffuseColor(float4(1.0f, 0.9f, 0.5f, 1.0f));
		light->setAttenuation(float3(0.0f, 0.15f, 0.0f));
		light->setSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
		light->setRange(100.0f);
		light->setShadows(true);

		auto transform = ecs_engine.getComponent<Transform>(point_light);
		transform->setPosition(float3(0.0f, 6.0f, 0.0f));
	}

	// Spot light
	{
		auto light = ecs_engine.getComponent<SpotLight>(spot_light);
		light->setAmbientColor(float4(0.15f, 0.15f, 0.15f, 1.0f));
		light->setDiffuseColor(float4(0.8f, 0.8f, 1.0f, 1.0f));
		light->setAttenuation(float3(0.05f, 0.15f, 0.0f));
		light->setSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
		light->setRange(100.0f);
		light->setUmbraAngle(XM_PI / 6.0f);
		light->setPenumbraAngle(XM_PI / 3.0f);
		light->setShadows(true);

		auto transform = ecs_engine.getComponent<Transform>(spot_light);
		transform->setPosition(float3(-1.0f, 0.0f, 0.0f));
		//transform->SetRotation(float3(XM_PIDIV2, 0.0f, 0.0f));
		transform->setParent(camera);
	}


	// Fog
	fog.color = float4(0.2f, 0.2f, 0.2f, 1.0f);
	fog.start = 25.0f;
	fog.range = 45.0f;


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	auto bp     = resource_mgr.getOrCreate<ModelBlueprint>(L"../data/models/test/test.obj");
	auto sphere = BlueprintFactory::CreateSphere<VertexPositionNormalTexture>(resource_mgr, 1.0f);

	const handle64 test_model = ecs_engine.createEntity<BasicModel>(device, bp);
	entities.push_back(test_model);


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------

	const wstring font(L"../data/fonts/courier-12.spritefont");

	texts.try_emplace("FPS", resource_mgr, font);
	texts.at("FPS").setPosition(float2(10, 10));

	texts.try_emplace("FrameTime", resource_mgr, font);
	texts.at("FrameTime").setPosition(float2(10, 40));

	texts.try_emplace("CPU", resource_mgr, font);
	texts.at("CPU").setPosition(float2(10, 70));

	texts.try_emplace("RAM", resource_mgr, font);
	texts.at("RAM").setPosition(float2(10, 140));

	texts.try_emplace("Mouse", resource_mgr, font);
	texts.at("Mouse").setPosition(float2(10, 210));
}


void TestScene::tick(const Engine& engine) {

	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	const int2 mouse_delta      = engine.getInput().getMouseDelta();
	const u32 fps               = engine.getFPSCounter().getFPS();
	const float delta_time      = engine.getTimer().deltaTime();
	const u64 total_cpu_usage   = engine.getSysMon().cpu().getTotalCpuPercentage();
	const double proc_cpu_usage = engine.getSysMon().cpu().getProcessCpuPercentage();
	const u64 total_mem_usage   = engine.getSysMon().memory().getTotalUsedPhysicalMem();
	const u64 proc_mem_usage    = engine.getSysMon().memory().getProcessUsedPhysicalMem();

	static wostringstream cpu_str;
	static wostringstream mem_str;

	cpu_str.clear();
	cpu_str.str(L"");
	cpu_str.precision(4);
	cpu_str << L"CPU Usage\nTotal: " << total_cpu_usage << L"%"
		<< L"\nProcess: " << proc_cpu_usage << L"%";

	mem_str.clear();
	mem_str.str(L"");
	mem_str.precision(4);
	mem_str << L"RAM Usage\nTotal: " << static_cast<double>(total_mem_usage) / 1e6 << L"MB"
		<< L"\nProcess: " << static_cast<double>(proc_mem_usage / 1e6) << L"MB";


	texts.at("FPS").setText(L"FPS: " + to_wstring(fps));

	texts.at("FrameTime").setText(L"Frame Time: " + to_wstring(delta_time));

	texts.at("CPU").setText(cpu_str.str());

	texts.at("RAM").setText(mem_str.str());

	texts.at("Mouse").setText(L"Mouse \nX: " + to_wstring(mouse_delta.x)
	                          + L"\nY: " + to_wstring(mouse_delta.y));
}

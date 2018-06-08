#include "stdafx.h"
#include "test_scene.h"
#include "engine\engine.h"


static constexpr float zNear = 0.1f;
static constexpr float zFar  = 1000.0f;
static constexpr float FOV   = XM_PI / 3.0f;


TestScene::TestScene(const Engine& engine) : Scene("Test Scene") {

	Init(engine);
}


void TestScene::Init(const Engine& engine) {

	auto& ecs_engine           = engine.GetECS();
	const auto& rendering_mgr  = engine.GetRenderingMgr();
	auto& device               = rendering_mgr.GetDevice();
	auto& device_context       = rendering_mgr.GetDeviceContext();

	auto& resource_mgr = rendering_mgr.GetResourceMgr();


	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	// Create the camera and skybox
	handle64 camera = ecs_engine.createEntity<PlayerCamera>(device,
															device_context,
															engine.GetWindowWidth(),
															engine.GetWindowHeight(),
															resource_mgr.GetOrCreate<Texture>(L"../data/Textures/grasscube1024.dds"));

	// Set the parameters
	auto cam = ecs_engine.getComponent<PerspectiveCamera>(camera);
	cam->SetZDepth(zNear, zFar);
	cam->SetFOV(FOV);
	ecs_engine.getComponent<CameraTransform>(camera)->SetPosition(float3(0.0f, 4.0f, -2.0f));

	entities.push_back(camera);


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------

	auto point_light = ecs_engine.createEntity<BasicPointLight>();
	entities.push_back(point_light);

	auto spot_light = ecs_engine.createEntity<BasicSpotLight>();
	entities.push_back(spot_light);

	// Point light
	{
		auto light = ecs_engine.getComponent<PointLight>(point_light);
		light->SetAmbientColor(float4(0.15f, 0.15f, 0.15f, 1.0f));
		light->SetDiffuseColor(float4(1.0f, 0.9f, 0.5f, 1.0f));
		light->SetAttenuation(float3(0.0f, 0.15f, 0.0f));
		light->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
		light->SetRange(100.0f);
		light->SetShadows(true);

		auto transform = ecs_engine.getComponent<Transform>(point_light);
		transform->SetPosition(float3(0.0f, 6.0f, 0.0f));

	}

	// Spot light
	{
		auto light = ecs_engine.getComponent<SpotLight>(spot_light);
		light->SetAmbientColor(float4(0.15f, 0.15f, 0.15f, 1.0f));
		light->SetDiffuseColor(float4(0.8f, 0.8f, 1.0f, 1.0f));
		light->SetAttenuation(float3(0.05f, 0.15f, 0.0f));
		light->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
		light->SetRange(100.0f);
		light->SetUmbraAngle(XM_PI / 6.0f);
		light->SetPenumbraAngle(XM_PI / 3.0f);
		light->SetShadows(true);

		auto transform = ecs_engine.getComponent<Transform>(spot_light);
		transform->SetPosition(float3(-1.0f, 0.0f, 0.0f));
		//transform->SetRotation(float3(XM_PIDIV2, 0.0f, 0.0f));
		transform->SetParent(camera);
	}


	// Fog
	fog.color = float4(0.2f, 0.2f, 0.2f, 1.0f);
	fog.start = 25.0f;
	fog.range = 45.0f;


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	auto bp     = resource_mgr.GetOrCreate<ModelBlueprint>(L"../data/models/test/test.obj");
	auto sphere = BlueprintFactory::CreateSphere<VertexPositionNormalTexture>(device, resource_mgr, 1.0f);

	handle64 test_model = ecs_engine.createEntity<BasicModel>(device, bp);
	entities.push_back(test_model);



	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------

	const wstring font(L"../data/fonts/courier-12.spritefont");

	texts.try_emplace("FPS", resource_mgr, font);
	texts.at("FPS").SetPosition(float2(10, 10));

	texts.try_emplace("FrameTime", resource_mgr, font);
	texts.at("FrameTime").SetPosition(float2(10, 40));

	texts.try_emplace("CPU", resource_mgr, font);
	texts.at("CPU").SetPosition(float2(10, 70));

	texts.try_emplace("RAM", resource_mgr, font);
	texts.at("RAM").SetPosition(float2(10, 140));

	texts.try_emplace("Mouse", resource_mgr, font);
	texts.at("Mouse").SetPosition(float2(10, 210));
}


void TestScene::Tick(const Engine& engine) {

	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	const int2   mouse_delta     = engine.GetInput().GetMouseDelta();
	const u32    fps             = engine.GetFPSCounter().GetFPS();
	const float  delta_time      = engine.GetTimer().DeltaTime();
	const u64    total_cpu_usage = engine.GetSysMon().CPU().GetTotalCpuPercentage();
	const double proc_cpu_usage  = engine.GetSysMon().CPU().GetProcessCpuPercentage();
	const u64    total_mem_usage = engine.GetSysMon().Memory().GetTotalUsedPhysicalMem();
	const u64    proc_mem_usage  = engine.GetSysMon().Memory().GetProcessUsedPhysicalMem();
	
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


	texts.at("FPS").SetText(L"FPS: " + to_wstring(fps));

	texts.at("FrameTime").SetText(L"Frame Time: " + to_wstring(delta_time));

	texts.at("CPU").SetText(cpu_str.str());

	texts.at("RAM").SetText(mem_str.str());

	texts.at("Mouse").SetText(L"Mouse \nX: " + to_wstring(mouse_delta.x)
							  + L"\nY: " + to_wstring(mouse_delta.y));
}
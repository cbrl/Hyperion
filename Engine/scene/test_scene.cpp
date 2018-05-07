#include "stdafx.h"
#include "test_scene.h"
#include "engine\engine.h"


static constexpr float zNear = 0.1f;
static constexpr float zFar = 1000.0f;
static constexpr float FOV = XM_PI / 3.0f;


TestScene::TestScene(const Engine& engine) : Scene("Test Scene") {

	Init(engine);
}


void TestScene::Init(const Engine& engine) {

	const auto& rendering_mgr  = engine.GetRenderingMgr();
	auto* device               = rendering_mgr.GetDevice();
	auto* device_context       = rendering_mgr.GetDeviceContext();

	auto& resource_mgr = rendering_mgr.GetResourceMgr();


	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	// Create the camera
	camera = ECS::Get()->CreateEntity<PlayerCamera>(device, device_context, engine.GetInputWeakPtr(), engine.GetWindowWidth(), engine.GetWindowHeight());

	// Create the skybox
	SkyBox skybox(device, resource_mgr, L"../data/Textures/grasscube1024.dds");

	// Set the parameters
	auto cam = ECS::Get()->GetComponent<PerspectiveCamera>(camera);
	cam->SetZDepth(zNear, zFar);
	cam->SetFOV(FOV);
	//camera->SetSkybox(skybox);
	//camera->SetPosition(float3(0.0f, 3.0f, -5.0f));


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------

	/*
	// X+ direction = Red
	directional_lights.push_back(DirectionalLightBuffer());
	directional_lights[0].ambient_color = float4(0.2f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].diffuse_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].specular      = float4(1.0f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].direction     = float3(1.0f, 0.0f, 0.0f);

	// Y- direction = Green
	directional_lights.push_back(DirectionalLightBuffer());
	directional_lights[1].ambient_color = float4(0.0f, 0.2f, 0.0f, 1.0f);
	directional_lights[1].diffuse_color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	directional_lights[1].specular      = float4(0.0f, 1.0f, 0.0f, 1.0f);
	directional_lights[1].direction     = float3(0.0f, -1.0f, 0.0f);

	// Z+ direction = Blue
	directional_lights.push_back(DirectionalLightBuffer());
	directional_lights[2].ambient_color = float4(0.0f, 0.0f, 0.2f, 1.0f);
	directional_lights[2].diffuse_color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	directional_lights[2].specular      = float4(0.0f, 0.0f, 1.0f, 1.0f);
	directional_lights[2].direction     = float3(0.0f, 0.0f, 1.0f);
	*/

	point_lights.push_back(PointLightBuffer());
	point_lights.back().ambient_color = float4(0.2f, 0.2f, 0.2f, 1.0f);
	point_lights.back().diffuse_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	point_lights.back().attenuation   = float3(0.0f, 0.15f, 0.0f);
	point_lights.back().specular      = float4(1.0f, 1.0f, 1.0f, 1.0f);
	point_lights.back().position      = float3(0.0f, 10.0f, 0.0f);
	point_lights.back().range         = 100.0f;


	// Fog
	fog.color = float4(0.2f, 0.2f, 0.2f, 1.0f);
	fog.start = 25.0f;
	fog.range = 45.0f;


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	auto bp = resource_mgr.GetOrCreate<ModelBlueprint>(L"../data/models/test/test.obj");
	models.push_back(Model(device, bp));
	//models.back().Scale(float3(3.0f, 3.0f, 3.0f));
	//models.back().SetPosition(5.0f, 0.0f, 0.0f);
	//models.back().SetRotation(0.0f, 1.2f, 0.0f);

	auto n = BlueprintFactory::CreateCube<VertexPositionNormalTexture>(device, resource_mgr, 5.0f);
	models.push_back(Model(device, n));


	Handle64 test_model = ECS::Get()->CreateEntity<BasicModel>(device, bp);
	entities.push_back(test_model);


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------

	const wstring font(L"../data/fonts/courier-12.spritefont");

	texts.try_emplace("FPS", resource_mgr, font);
	texts.at("FPS").SetPosition(float2(10, 10));

	texts.try_emplace("Mouse", resource_mgr, font);
	texts.at("Mouse").SetPosition(float2(10, 40));

	texts.try_emplace("Position", resource_mgr, font);
	texts.at("Position").SetPosition(float2(10, 110));

	texts.try_emplace("Rotation", resource_mgr, font);
	texts.at("Rotation").SetPosition(float2(10, 200));

	texts.try_emplace("Velocity", resource_mgr, font);
	texts.at("Velocity").SetPosition(float2(10, 300));
}


void TestScene::Tick(const Engine& engine) {

	// Get input handler
	const Input& input = engine.GetInput();

	// Get mouse delta
	int2 mouse_delta = input.GetMouseDelta();



	// Rotate models
	//for (auto& model : models) {
	//	model.Rotate(0.0f, ((XM_PI * delta_time) / 2500), 0.0f);
	//}

	

	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	u32 fps = engine.GetFPSCounter().GetFPS();

	texts.at("FPS").SetText(L"FPS: " + to_wstring(fps));

	texts.at("Mouse").SetText(L"Mouse \nX: " + to_wstring(mouse_delta.x)
							  + L"\nY: " + to_wstring(mouse_delta.y));

	//float3 position = camera->GetPosition();
	//texts.at("Position").SetText(L"Position \nX: " + to_wstring(position.x)
	//							 + L"\nY: " + to_wstring(position.y)
	//							 + L"\nZ: " + to_wstring(position.z));

	//float3 rotation = camera->GetRotation();
	//texts.at("Rotation").SetText(L"Rotation \nX: " + to_wstring(rotation.x)
	//							 + L"\nY: " + to_wstring(rotation.y)
	//							 + L"\nZ: " + to_wstring(rotation.z));

	//float3 velocity = camera->GetVelocity() * 1000.0f;
	//texts.at("Velocity").SetText(L"Velocity \nX: " + to_wstring(velocity.x)
	//							 + L"\nY: " + to_wstring(velocity.y)
	//							 + L"\nZ: " + to_wstring(velocity.z));
}
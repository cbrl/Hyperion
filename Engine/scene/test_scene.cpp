#include "stdafx.h"
#include "test_scene.h"
#include "system\system.h"


static constexpr float zNear = 0.1f;
static constexpr float zFar = 1000.0f;
static constexpr float FOV = XM_PI / 3.0f;


TestScene::TestScene(ID3D11Device* device,
					 ID3D11DeviceContext* device_context,
					 ResourceMgr& resource_mgr) {
	Init(device, device_context, resource_mgr);
}


void TestScene::Init(ID3D11Device* device,
					 ID3D11DeviceContext* device_context,
					 ResourceMgr& resource_mgr) {

	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------

	camera = make_unique<Camera>(device,
								 resource_mgr,
								 System::Get()->GetWindowWidth(),
								 System::Get()->GetWindowHeight(),
								 FOV, zNear, zFar,
								 L"../data/Textures/grasscube1024.dds");

	camera->SetPosition(float3(0.0f, 1.0f, -5.0f));


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------

	/*
	// X+ direction = Red
	directional_lights.push_back(DirectionalLight());
	directional_lights[0].ambient_color = float4(0.2f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].diffuse_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].specular      = float4(1.0f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].direction     = float3(1.0f, 0.0f, 0.0f);

	// Y- direction = Green
	directional_lights.push_back(DirectionalLight());
	directional_lights[1].ambient_color = float4(0.0f, 0.2f, 0.0f, 1.0f);
	directional_lights[1].diffuse_color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	directional_lights[1].specular      = float4(0.0f, 1.0f, 0.0f, 1.0f);
	directional_lights[1].direction     = float3(0.0f, -1.0f, 0.0f);

	// Z+ direction = Blue
	directional_lights.push_back(DirectionalLight());
	directional_lights[2].ambient_color = float4(0.0f, 0.0f, 0.2f, 1.0f);
	directional_lights[2].diffuse_color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	directional_lights[2].specular      = float4(0.0f, 0.0f, 1.0f, 1.0f);
	directional_lights[2].direction     = float3(0.0f, 0.0f, 1.0f);
	*/

	point_lights.push_back(PointLight());
	point_lights.back().ambient_color = float4(0.2f, 0.2f, 0.2f, 1.0f);
	point_lights.back().diffuse_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	point_lights.back().attenuation = float3(0.5f, 0.0f, 0.1f);
	point_lights.back().specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	point_lights.back().position = float3(0.0f, 1.0f, -2.0f);
	point_lights.back().range = 50.0f;


	// Fog
	fog.color = float4(0.4f, 0.4f, 0.4f, 1.0f);
	fog.start = 25.0f;
	fog.range = 45.0f;


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	auto bp = resource_mgr.Create<ModelBlueprint>(L"../data/models/test/", L"test.obj");
	models.push_back(Model(device, *bp));
	models.back().Scale(3.0f, 3.0f, 3.0f);
	//models.back().SetPosition(5.0f, 0.0f, 0.0f);
	//models.back().SetRotation(0.0f, 1.2f, 0.0f);


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------

	const wstring font(L"../data/fonts/courier-12.spritefont");

	texts.try_emplace("FPS", device_context, resource_mgr, font);
	texts.at("FPS").SetPosition(float2(10, 10));

	texts.try_emplace("Mouse", device_context, resource_mgr, font);
	texts.at("Mouse").SetPosition(float2(10, 40));

	texts.try_emplace("Position", device_context, resource_mgr, font);
	texts.at("Position").SetPosition(float2(10, 110));

	texts.try_emplace("Rotation", device_context, resource_mgr, font);
	texts.at("Rotation").SetPosition(float2(10, 200));

	texts.try_emplace("Velocity", device_context, resource_mgr, font);
	texts.at("Velocity").SetPosition(float2(10, 300));
}


void TestScene::Tick(float delta_time) {

	// Get input handler
	const Input& input = System::Get()->GetInput();

	// Get mouse delta
	i32 mouse_x, mouse_y;
	input.GetMouseDelta(mouse_x, mouse_y);



	// Rotate models
	//for (auto& model : models) {
	//	model.Rotate(0.0f, ((XM_PI * delta_time) / 2500), 0.0f);
	//}

	

	//----------------------------------------------------------------------------------
	// Update FPS, CPU usage, memory usage, mouse position, etc...
	//----------------------------------------------------------------------------------

	u32 fps = System::Get()->GetFPSCounter().GetFPS();

	texts.at("FPS").SetText(L"FPS: " + to_wstring(fps));

	texts.at("Mouse").SetText(L"Mouse \nX: " + to_wstring(mouse_x)
							  + L"\nY: " + to_wstring(mouse_y));

	float3 position = camera->GetPosition();
	texts.at("Position").SetText(L"Position \nX: " + to_wstring(position.x)
								 + L"\nY: " + to_wstring(position.y)
								 + L"\nZ: " + to_wstring(position.z));

	float3 rotation = camera->GetRotation();
	texts.at("Rotation").SetText(L"Rotation \nX: " + to_wstring(rotation.x)
								 + L"\nY: " + to_wstring(rotation.y)
								 + L"\nZ: " + to_wstring(rotation.z));

	float3 velocity = camera->GetVelocity() * 1000.0f;
	texts.at("Velocity").SetText(L"Velocity \nX: " + to_wstring(velocity.x)
								 + L"\nY: " + to_wstring(velocity.y)
								 + L"\nZ: " + to_wstring(velocity.z));


	//----------------------------------------------------------------------------------
	// Camera movement
	//----------------------------------------------------------------------------------

	// Exit if input is disabled
	if (!enable_input) return;

	float3 rotateUnits(0.0f, 0.0f, 0.0f);
	float3 move_units(0.0f, 0.0f, 0.0f);

	// Set x/y rotation with mouse data
	rotateUnits.x = static_cast<float>(mouse_y);
	rotateUnits.y = static_cast<float>(mouse_x);

	// Roll rotation
	if (input.IsKeyDown(Keyboard::Q)) {
		rotateUnits.z -= delta_time;
	}
	else if (input.IsKeyDown(Keyboard::E)) {
		rotateUnits.z += delta_time;
	}

	// Forward/Back movement
	if (input.IsKeyDown(Keyboard::W)) {
		move_units.z += delta_time;
	}
	else if (input.IsKeyDown(Keyboard::S)) {
		move_units.z -= delta_time;
	}

	// Left/Right movement
	if (input.IsKeyDown(Keyboard::A)) {
		move_units.x -= delta_time;
	}
	else if (input.IsKeyDown(Keyboard::D)) {
		move_units.x += delta_time;
	}

	// Up/Down movement
	if (input.IsKeyDown(Keyboard::Space)) {
		move_units.y += delta_time;
	}
	else if (input.IsKeyDown(Keyboard::LeftControl)) {
		move_units.y -= delta_time;
	}

	// Update camera rotation and position
	camera->Move(move_units);
	camera->Rotate(rotateUnits);

	// Update the camera position/rotation
	camera->UpdateMovement(delta_time);
}
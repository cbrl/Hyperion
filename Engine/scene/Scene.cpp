#include "stdafx.h"
#include "Scene.h"
#include "rendering\renderingMgr.h"

// TODO:
// - Add vertex normal generation to obj loader
// - Create a simple geometry generator
// - Global shader include
// - Shader buffers for different lights and number of them
// - Implement material data into shaders
// - Pipleline class for binding resources to pipline stages

Scene::Scene() {
	Init(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());
}


Scene::~Scene() {
}


void Scene::Render(float deltaTime) {
	Renderer::Get()->Tick(*this, deltaTime);
}


void Scene::Init(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------
	camera = make_unique<Camera>();
	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------
	lights.push_back(Light());
	lights[0].SetDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	lights[0].SetAmbientColor(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f));
	lights[0].SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	lights[0].SetSpecularColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	lights[0].SetSpecularPower(64.0f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------
	OBJLoader loader;
	models.push_back(loader.Load(device, device_context, L"data/models/cube2/", L"cube.obj", false));
	models.at(0).SetShader(ShaderTypes::LightShader);


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------
	const wchar_t* font = L"./data/fonts/courier-12.spritefont";

	texts.try_emplace("FPS", device, device_context, font);
	texts.at("FPS").SetPosition(XMFLOAT2(10, 10));

	texts.try_emplace("Mouse", device, device_context, font);
	texts.at("Mouse").SetPosition(XMFLOAT2(10, 40));

	texts.try_emplace("Position", device, device_context, font);
	texts.at("Position").SetPosition(XMFLOAT2(10, 110));
	
	texts.try_emplace("Rotation", device, device_context, font);
	texts.at("Rotation").SetPosition(XMFLOAT2(10, 200));

	texts.try_emplace("Velocity", device, device_context, font);
	texts.at("Velocity").SetPosition(XMFLOAT2(10, 300));
}


void Scene::UpdateMetrics(int FPS, int CPU, int mouseX, int mouseY) {
	// FPS, CPU usage, memory usage, mouse position, etc...

	texts.at("FPS").SetText(L"FPS: " + to_wstring(FPS));

	texts.at("Mouse").SetText(L"Mouse \nX: " + to_wstring(mouseX)
	                           + L"\nY: " + to_wstring(mouseY));

	XMFLOAT3 position = camera->GetPosition();
	texts.at("Position").SetText(L"Position \nX: " + to_wstring(position.x)
	                              + L"\nY: " + to_wstring(position.y)
	                              + L"\nZ: " + to_wstring(position.z));

	XMFLOAT3 rotation = camera->GetRotation();
	texts.at("Rotation").SetText(L"Rotation \nX: " + to_wstring(rotation.x)
	                              + L"\nY: " + to_wstring(rotation.y)
	                              + L"\nZ: " + to_wstring(rotation.z));

	XMFLOAT3 velocity = camera->GetVelocity();
	texts.at("Velocity").SetText(L"Velocity \nX: " + to_wstring(velocity.x)
	                              + L"\nY: " + to_wstring(velocity.y)
	                              + L"\nZ: " + to_wstring(velocity.z));
}


void Scene::Tick(Input& input, float deltaTime) {

	// Rotate models
	for (auto& model : models) {
		model.Rotate(0.0f, ((XM_PI * deltaTime) / 2500), 0.0f);
	}


	//----------------------------------------------------------------------------------
	// Camera movement
	//----------------------------------------------------------------------------------
	int mouseX, mouseY;
	XMFLOAT3 rotateUnits(0.0f, 0.0f, 0.0f);
	XMFLOAT3 move_units(0.0f, 0.0f, 0.0f);

	// Get mouse state
	input.GetMouseDelta(mouseX, mouseY);

	// Set x/y rotation with mouse data
	rotateUnits.x = mouseY;
	rotateUnits.y = mouseX;

	// Roll rotation
	if (input.IsKeyDown(Keyboard::Q)) {
		rotateUnits.z -= deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::E)) {
		rotateUnits.z += deltaTime;
	}

	// Forward/Back movement
	if (input.IsKeyDown(Keyboard::W)) {
		move_units.z += deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::S)) {
		move_units.z -= deltaTime;
	}

	// Left/Right movement
	if (input.IsKeyDown(Keyboard::A)) {
		move_units.x -= deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::D)) {
		move_units.x += deltaTime;
	}

	// Up/Down movement
	if (input.IsKeyDown(Keyboard::Space)) {
		move_units.y += deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::LeftControl)) {
		move_units.y -= deltaTime;
	}

	// Update camera rotation and position
	if (move_units.x || move_units.y || move_units.z) {
		camera->Move(move_units);
	}
	if (rotateUnits.x || rotateUnits.y || rotateUnits.z) {
		camera->Rotate(rotateUnits);
	}

	// Update camera 
	camera->Update(deltaTime);
}
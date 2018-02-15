#include "stdafx.h"
#include "Scene.h"
#include "rendering\renderingMgr.h"

// TODO:
// - Add vertex normal generation to obj loader
// - Create a simple geometry generator
// - Global shader include
// - Implement material data into shaders

Scene::Scene() {
	Init(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());
}


Scene::~Scene() {
}


void Scene::Render(float deltaTime) {
	Renderer::Get()->Tick(*this, deltaTime);
}


void Scene::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
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
	lights[0].SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	lights[0].SetSpecularPower(64.0f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------
	OBJLoader loader;
	models.push_back(loader.Load(device, deviceContext, L"data/models/cube2/", L"cube.obj", false));
	models.at(0).SetShader(ShaderTypes::LightShader);


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------
	const wchar_t* font = L"./data/fonts/courier-12.spritefont";

	texts.try_emplace("FPS", device, deviceContext, font);
	texts.at("FPS").SetPosition(XMFLOAT2(10, 10));

	texts.try_emplace("Mouse", device, deviceContext, font);
	texts.at("Mouse").SetPosition(XMFLOAT2(10, 40));

	texts.try_emplace("Position", device, deviceContext, font);
	texts.at("Position").SetPosition(XMFLOAT2(10, 110));
	
	texts.try_emplace("Rotation", device, deviceContext, font);
	texts.at("Rotation").SetPosition(XMFLOAT2(10, 200));

	texts.try_emplace("Velocity", device, deviceContext, font);
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
	rotation += (XM_PI * deltaTime) / 2500;
	if (rotation >= (2.0f * XM_PI)) rotation = 0;
	//models.at(0).SetRotation(0.0f, rotation, 0.0f);
	//models.at(1).SetRotation(rotation, 0.0f, 0.0f);
	//for (auto& model : models) {
	//	model.SetRotation(0.0f, rotation, 0.0f);
	//}


	//----------------------------------------------------------------------------------
	// Camera movement
	//----------------------------------------------------------------------------------
	int mouseX, mouseY;
	XMFLOAT3 rotateUnits(0.0f, 0.0f, 0.0f);
	XMFLOAT3 moveUnits(0.0f, 0.0f, 0.0f);

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
		moveUnits.z += deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::S)) {
		moveUnits.z -= deltaTime;
	}

	// Left/Right movement
	if (input.IsKeyDown(Keyboard::A)) {
		moveUnits.x -= deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::D)) {
		moveUnits.x += deltaTime;
	}

	// Up/Down movement
	if (input.IsKeyDown(Keyboard::Space)) {
		moveUnits.y += deltaTime;
	}
	else if (input.IsKeyDown(Keyboard::LeftControl)) {
		moveUnits.y -= deltaTime;
	}

	// Update camera rotation and position
	if (moveUnits.x || moveUnits.y || moveUnits.z) {
		camera->Move(moveUnits);
	}
	if (rotateUnits.x || rotateUnits.y || rotateUnits.z) {
		camera->Rotate(rotateUnits);
	}

	// Update camera 
	camera->Update(deltaTime);
}
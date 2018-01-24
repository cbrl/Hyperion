#include "stdafx.h"
#include "Scene.h"

// TODO:
// - PARTIAL - Method for setting sampler state
// - Implement material data into shaders
// - Create frustum and implement frustum culling
// - Create a model loader, and a model class that can take any vertex type
// - Create a simple geometry generator
// - Bounding boxes on geometry for frustum culling
// - Global shader include

Scene::Scene(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	hWnd(hWnd),
	device(device),
	deviceContext(deviceContext)
{
}


Scene::~Scene() {
}


bool Scene::Init() {
	bool result;

	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------
	camera = make_unique<Camera>();
	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));


	//----------------------------------------------------------------------------------
	// Create texture manager
	//----------------------------------------------------------------------------------
	textureMgr = make_unique<TextureMgr>(device, deviceContext);


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------
	lights.push_back(Light());
	lights[0].SetDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	lights[0].SetAmbientColor(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f));
	lights[0].SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	lights[0].SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	lights[0].SetSpecularPower(32.0f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------
	models.push_back(Model());
	vector<wstring> v;
	v.push_back(L"./data/brick.jpg");
	//v.push_back(L"./data/brick2.jpg");
	result = models[0].Init(device.Get(), "./data/cube.txt", textureMgr->Texture(v), ShaderTypes::LightShader);
	//result = models[0].Init(device.Get(), "./data/cube.txt", textureMgr->SimpleTexture(XMFLOAT4(Colors::Aqua)), ShaderTypes::LightShader);
	if (!result) return false;


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------
	texts.try_emplace(L"FPS", device.Get(), deviceContext.Get(), L"./data/courier-12.spritefont");
	texts.at(L"FPS").SetPosition(XMFLOAT2(10, 10));

	texts.try_emplace(L"Mouse", device.Get(), deviceContext.Get(), L"./data/courier-12.spritefont");
	texts.at(L"Mouse").SetPosition(XMFLOAT2(10, 40));

	texts.try_emplace(L"Position", device.Get(), deviceContext.Get(), L"./data/courier-12.spritefont");
	texts.at(L"Position").SetPosition(XMFLOAT2(10, 110));
	
	texts.try_emplace(L"Rotation", device.Get(), deviceContext.Get(), L"./data/courier-12.spritefont");
	texts.at(L"Rotation").SetPosition(XMFLOAT2(10, 200));

	texts.try_emplace(L"Velocity", device.Get(), deviceContext.Get(), L"./data/courier-12.spritefont");
	texts.at(L"Velocity").SetPosition(XMFLOAT2(10, 300));

	return true;
}


void Scene::UpdateMetrics(int FPS, int CPU, int mouseX, int mouseY) {
	// FPS, CPU usage, memory usage, mouse position, etc...

	texts.at(L"FPS").SetText(L"FPS: " + to_wstring(FPS));

	texts.at(L"Mouse").SetText(L"Mouse \nX: " + to_wstring(mouseX)
	                           + L"\nY: " + to_wstring(mouseY));

	texts.at(L"Position").SetText(L"Position \nX: " + to_wstring(camera->GetPosition().x)
	                              + L"\nY: " + to_wstring(camera->GetPosition().y)
	                              + L"\nZ: " + to_wstring(camera->GetPosition().z));

	texts.at(L"Rotation").SetText(L"Rotation \nX: " + to_wstring(camera->GetRotation().x)
	                              + L"\nY: " + to_wstring(camera->GetRotation().y)
	                              + L"\nZ: " + to_wstring(camera->GetRotation().z));

	texts.at(L"Velocity").SetText(L"Velocity \nX: " + to_wstring(camera->GetVelocity().x)
	                              + L"\nY: " + to_wstring(camera->GetVelocity().y)
	                              + L"\nZ: " + to_wstring(camera->GetVelocity().z));
}


void Scene::Tick(Input& input, float deltaTime) {

	// Rotate models
	rotation += (XM_PI * deltaTime) / 2500;
	if (rotation >= (2.0f * XM_PI)) rotation = 0;
	for (auto model : models) {
		model.SetRotation(0.0f, rotation, 0.0f);
	}


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
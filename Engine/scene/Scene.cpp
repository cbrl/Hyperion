#include "stdafx.h"
#include "scene.h"
#include "system\system.h"
#include "rendering\rendering_mgr.h"

// TODO:
// - Clean code
// - Add vertex normal generation to obj loader
// - Create a simple geometry generator
// - Pipleline class for binding resources to pipline stages
// - Replace UINT and unsigned int with uint32_t


static const float zNear = 0.1f;
static const float zFar  = 1000.0f;
static const float FOV   = XM_PI / 3.0f;


Scene::Scene() {
	Init(RenderingMgr::GetDevice(), RenderingMgr::GetDeviceContext());
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

	camera = make_unique<Camera>(System::Get()->GetWindowWidth(), System::Get()->GetWindowHeight(),
								 FOV, zNear, zFar);
	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------

	// X+ direction = Red
	directional_lights.push_back(DirectionalLight());
	directional_lights[0].ambient_color = XMFLOAT4(0.2f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].diffuse_color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	directional_lights[0].specular      = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	directional_lights[0].direction     = XMFLOAT3(1.0f, 0.0f, 0.0f);

	// Y- direction = Green
	directional_lights.push_back(DirectionalLight());
	directional_lights[1].ambient_color = XMFLOAT4(0.0f, 0.2f, 0.0f, 1.0f);
	directional_lights[1].diffuse_color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	directional_lights[1].specular      = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	directional_lights[1].direction     = XMFLOAT3(0.0f, -1.0f, 0.0f);

	// Z+ direction = Blue
	directional_lights.push_back(DirectionalLight());
	directional_lights[2].ambient_color = XMFLOAT4(0.0f, 0.0f, 0.2f, 1.0f);
	directional_lights[2].diffuse_color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	directional_lights[2].specular      = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	directional_lights[2].direction     = XMFLOAT3(0.0f, 0.0f, 1.0f);

	//point_lights.push_back(PointLight());
	//point_lights.back().ambient_color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	//point_lights.back().diffuse_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//point_lights.back().attenuation   = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//point_lights.back().specular      = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	//point_lights.back().position      = XMFLOAT3(0.0f, 0.0f, -2.0f);
	//point_lights.back().range         = 10.0f;


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------

	OBJLoader loader;
	models.push_back(loader.Load(device, device_context, L"data/models/spaceCompound/", L"spaceCompound.obj", false));


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
	//for (auto& model : models) {
	//	model.Rotate(0.0f, ((XM_PI * deltaTime) / 2500), 0.0f);
	//}


	//----------------------------------------------------------------------------------
	// Camera movement
	//----------------------------------------------------------------------------------
	int mouseX, mouseY;
	XMFLOAT3 rotateUnits(0.0f, 0.0f, 0.0f);
	XMFLOAT3 move_units(0.0f, 0.0f, 0.0f);

	// Get mouse state
	input.GetMouseDelta(mouseX, mouseY);

	// Set x/y rotation with mouse data
	rotateUnits.x = static_cast<float>(mouseY);
	rotateUnits.y = static_cast<float>(mouseX);

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
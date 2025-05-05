module;

#include "datatypes/types.h"
#include "memory/handle/handle.h"

#include "directx/d3d11.h"

#include "imgui.h"

export module test_scene;

import math.directxmath;

import rendering.blueprint_factory;
import rendering.engine;
import rendering.rendering_mgr;
import rendering.resource_mgr;
import rendering.scene;

// Entities
import entities.core_entities;
import entities.player_camera;

// Components
import components.core_components;
import components.motor.axis_orbit;
import components.motor.axis_rotation;
import components.motor.camera_movement;
import components.motor.mouse_rotation;

// Systems
import systems.core_systems;
import systems.motor.axis_orbit;
import systems.motor.axis_rotation;
import systems.motor.camera_motor;
import systems.motor.mouse_rotation;


export class TestScene : public render::Scene {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TestScene() : Scene("Test Scene") {
	}

	TestScene(const TestScene&) = delete;
	TestScene(TestScene&&) = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~TestScene() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TestScene& operator=(const TestScene&) = delete;
	TestScene& operator=(TestScene&&) = default;

protected:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void initialize(render::Engine& engine) {

		using namespace render;
		using namespace EntityTemplates;
	
		auto& input          = engine.getInput();
		auto& key_config     = engine.getKeyConfig();
		auto& rendering_mgr  = engine.getRenderingMgr();
		auto& device         = rendering_mgr.getDevice();
		auto& device_context = rendering_mgr.getDeviceContext();
		auto& resource_mgr   = rendering_mgr.getResourceMgr();

		auto& ecs            = getECS();

		//----------------------------------------------------------------------------------
		// Add systems
		//----------------------------------------------------------------------------------

		// User Interface
		auto& ui = ecs.add<render::systems::UserInterface>(engine);
		addUserComponentsToUI(ui);

		// Picking System: allows the user to select an object on the screen with the mouse
		ecs.add<render::systems::PickingSystem>(engine);

		// Camera motor system: moves an entity with a camera and camera movement component (entity requires CameraMovement component)
		ecs.add<CameraMotorSystem>(input, key_config);

		// Mouse rotation system: reads mouse input to rotate an entity (entity requires MouseRotation component)
		ecs.add<MouseRotationSystem>(input);

		// Axis rotation system: rotates an entity on an axis (entity requires AxisRotation component)
		ecs.add<AxisRotationSystem>();

		// Axis orbit system: orbits an entity around an axis (entity requires AxisOrbit component)
		ecs.add<AxisOrbitSystem>();


		//----------------------------------------------------------------------------------
		// Camera
		//----------------------------------------------------------------------------------

		// Create the camera
		handle64 camera = createEntity<PlayerCamera>(device, engine.getWindow().getClientSize());
		ecs.add<Name>(camera, "Camera");
		ecs.add<Hierarchy>(camera);
		ecs.add<AmbientLight>(camera).setColor(f32_3{0.16f, 0.16f, 0.16f});

		// Set the parameters
		auto& cam = ecs.get<PerspectiveCamera>(camera);

		// View settings
		cam.getViewport().setDepth(0.0f, 1.0f);
		cam.setZDepth(0.01f, 1000.0f);
		cam.setFOV(XM_PI / 3.0f);

		// Skybox
		cam.getSettings().setSkybox(resource_mgr.getOrCreate<Texture>(L"../data/Textures/grasscube1024.dds"));

		// Fog
		auto& fog = cam.getSettings().getFog();
		fog.density = 0.05f;
		fog.color = {0.2f, 0.2f, 0.2f};

		// Initial position
		ecs.get<Transform>(camera).setPosition(f32_3{0.0f, 3.0f, -3.0f});

		// Mouse control settings
		ecs.get<MouseRotation>(camera).setSensitivity(0.01f);


		//----------------------------------------------------------------------------------
		// Models
		//----------------------------------------------------------------------------------

		// Model config
		ModelConfig<VertexPositionNormalTexture> config;
		config.flip_winding = false;
		config.flip_uv      = false;

		// Inverted model config
		ModelConfig<VertexPositionNormalTexture> inv_config;
		inv_config.flip_winding = true;
		inv_config.flip_uv      = false;

		// Create blueprints
		//auto main_bp          = resource_mgr.getOrCreate<ModelBlueprint>(L"../data/models/test/test.obj", config);
		auto sphere_bp        = BlueprintFactory::CreateSphere(resource_mgr, config, 1.0f);
		auto cube_bp          = BlueprintFactory::CreateCube(resource_mgr, config, 2.0f);
		auto inverted_cube_bp = BlueprintFactory::CreateCube(resource_mgr, inv_config, 10.0f);
		auto cylinder_bp      = BlueprintFactory::CreateCylinder(resource_mgr, config, 2.0f, 2.0f);

		// Bounding Cube
		{
			handle64 inv_cube = importModel(device, inverted_cube_bp);
			ecs.add<Name>(inv_cube, "Bounding Cube");
			ecs.get<Transform>(inv_cube).setPosition(f32_3{0.0f, 5.0f, 0.0f});

			auto& mat = inverted_cube_bp->materials[0];
			mat.params.base_color = {0.2f, 0.2f, 0.8f, 1.0f};
			mat.params.roughness  = 1;
			mat.params.metalness  = 0;
		}

		// Cube
		{
			handle64 cube = importModel(device, cube_bp);
			ecs.add<Name>(cube, "Cube");
			ecs.get<Transform>(cube).setPosition(f32_3{-2.5f, 1.0f, 1.5f});

			auto& mat = cube_bp->materials[0];
			mat.params.base_color = {1.0f, 0.0f, 0.0f, 1.0f};
			mat.params.roughness  = 0.8f;
			mat.params.metalness  = 0;
		}

		// Cylinder
		{
			handle64 cylinder = importModel(device, cylinder_bp);
			ecs.add<Name>(cylinder, "Cylinder");
			ecs.get<Transform>(cylinder).setPosition(f32_3{2.5f, 1.0f, 0.75f});

			auto& mat = cylinder_bp->materials[0];
			mat.params.base_color = {0.0f, 1.0f, 0.0f, 1.0f};
			mat.params.roughness  = 0.35f;
			mat.params.metalness  = 0.95;
		}


		// Sphere
		{
			handle64 sphere = importModel(device, sphere_bp);
			ecs.add<Name>(sphere, "Sphere");
			ecs.get<Transform>(sphere).setPosition(f32_3{0.0f, 1.0f, -1.0f});
			ecs.get<Transform>(sphere).setScale(f32_3{2.0f});

			auto& mat = sphere_bp->materials[0];
			mat.params.base_color = {1.0f, 0.7f, 0.0f, 1.0f};
			mat.params.roughness  = 0.15f;
			mat.params.metalness  = 0;
		}


		// Sphere Light
		//auto sphere_light = importModel(device, sphere_bp);
		//ecs.get(sphere_light).setName("Sphere Light");
		//ecs.add<PointLight>(sphere_light);
		//ecs.get<Transform>(sphere_light).setPosition(f32_3{0.0f, 4.0f, 0.0f});

		//auto& rotation = ecs.add<AxisRotation>(sphere_light);
		//rotation.setAxis(AxisRotation::Axis::Y);
		//rotation.setSpeedY(1.5f);

		//auto* orbit = ecs.add<AxisOrbit>(sphere_light);
		//orbit.setSpeed(0.5f);


		//----------------------------------------------------------------------------------
		// Lights
		//----------------------------------------------------------------------------------

		// Sphere light
		{
			auto entity = createEntity();
			ecs.add<Name>(entity, "Point Light");
			ecs.get<Transform>(entity).setPosition(f32_3{0.0f, 4.0f, 0.0f});

			auto& light = ecs.add<PointLight>(entity);
			light.setBaseColor(f32_3{1.0f, 1.0f, 1.0f});
			light.setIntensity(9.0f);
			light.setAttenuation(f32_3{0.0f, 0.1f, 0.1f});
			light.setRange(100.0f);
			light.setShadows(true);
		}

		// Camera light
		{
			const auto light = createEntity<HierarchyT>();
			ecs.add<Name>(light, "Camera Light");
			ecs.get<Hierarchy>(camera).addChild(ecs, light);

			auto& spot_light = ecs.add<SpotLight>(light);
			spot_light.setBaseColor(f32_3{0.9f, 0.9f, 0.9f});
			spot_light.setIntensity(7.0f);
			spot_light.setAttenuation(f32_3{0.0f, 0.1f, 0.1f});
			spot_light.setRange(100.0f);
			spot_light.setUmbraAngle(XM_PI / 6.0f);
			spot_light.setPenumbraAngle(XM_PI / 4.0f);
			spot_light.setShadows(true);

			auto& transform = ecs.get<Transform>(light);
			transform.setPosition(f32_3{-1.0f, 0.0f, 0.0f});
		}

		// Directional Light
		{
			//const auto dir_light = createEntity();
			//auto* light = ecs->add<DirectionalLight>(dir_light);
			//light->setDiffuseColor(f32_4{ 0.0f, 0.0f, 1.0f, 0.0f });
			//light->setRange(100.0f);
			//light->setSize(f32_2{ 30.0f, 30.0f });

			//auto transform = ecs_engine.get<Transform>(dir_light);
			//transform->setPosition(f32_3{ 0.0f, 4.0f, 0.0f });
			//transform->setRotation(f32_3{ XM_PIDIV4, 0.0f, 0.0f });
		}


		//----------------------------------------------------------------------------------
		// Text objects
		//----------------------------------------------------------------------------------

		auto font = resource_mgr.getOrCreate<Font>(L"../data/fonts/courier-12.spritefont");

		scene_name_text = createEntity();
		ecs.add<Name>(scene_name_text, "Scene Name Text");
		ecs.add<Text>(scene_name_text, font);
		ecs.get<Text>(scene_name_text).setText(StrToWstr(this->getName()));
		ecs.get<Transform>(scene_name_text).setPosition(f32_3{10, 10, 0});
	}


	void update(render::Engine& engine) {

	}

private:

	void addUserComponentsToUI(render::systems::UserInterface& ui) {

		using namespace render::systems;

		//----------------------------------------------------------------------------------
		// Camera Movement
		//----------------------------------------------------------------------------------
		UserInterface::UserComponent camera_movement;

		camera_movement.name = "Camera Movement";

		camera_movement.getter = [](ecs::ECS& ecs, handle64 entity) -> typename ecs::IComponent* {
			return ecs.tryGet<CameraMovement>(entity);
		};

		camera_movement.adder = [](ecs::ECS& ecs, handle64 entity) -> void {
			ecs.add<CameraMovement>(entity);
		};

		camera_movement.details_renderer = [](ecs::IComponent& component) -> void {
			CameraMovement& movement = static_cast<CameraMovement&>(component);

			f32 max_velocity = movement.getMaxVelocity();
			f32 acceleration = movement.getAcceleration();
			f32 deceleration = movement.getDeceleration();

			if (ImGui::InputFloat("Max Velocity", &max_velocity))
				movement.setMaxVelocity(max_velocity);

			if (ImGui::InputFloat("Acceleration", &acceleration))
				movement.setAcceleration(acceleration);

			if (ImGui::InputFloat("Deceleration", &deceleration))
				movement.setDeceleration(deceleration);
		};

		ui.registerUserComponent<CameraMovement>(camera_movement);


		//----------------------------------------------------------------------------------
		// Mouse Rotation
		//----------------------------------------------------------------------------------
		UserInterface::UserComponent mouse_rotation;

		mouse_rotation.name = "Mouse Rotation";

		mouse_rotation.getter = [](ecs::ECS& ecs, handle64 entity) -> typename ecs::IComponent* {
			return ecs.tryGet<MouseRotation>(entity);
		};

		mouse_rotation.adder = [](ecs::ECS& ecs, handle64 entity) -> void {
			ecs.add<MouseRotation>(entity);
		};

		mouse_rotation.details_renderer = [](ecs::IComponent& component) -> void {
			MouseRotation& rotation = static_cast<MouseRotation&>(component);
			f32 sensitivity = rotation.getSensitivity();

			if (ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f, 0.0f, FLT_MAX)) {
				rotation.setSensitivity(sensitivity);
			}
		};

		ui.registerUserComponent<MouseRotation>(mouse_rotation);


		//----------------------------------------------------------------------------------
		// Axis Rotation
		//----------------------------------------------------------------------------------
		UserInterface::UserComponent axis_rotation;

		axis_rotation.name = "Axis Rotation";

		axis_rotation.getter = [](ecs::ECS& ecs, handle64 entity) -> typename ecs::IComponent* {
			return ecs.tryGet<AxisRotation>(entity);
		};

		axis_rotation.adder = [](ecs::ECS& ecs, handle64 entity) -> void {
			ecs.add<AxisRotation>(entity);
		};

		axis_rotation.details_renderer = [](ecs::IComponent& component) -> void {
			AxisRotation& rotation = static_cast<AxisRotation&>(component);

			//----------------------------------------------------------------------------------
			// X Rotation
			//----------------------------------------------------------------------------------
			bool x_enable = rotation.hasAxis(AxisRotation::Axis::X);

			if (ImGui::Checkbox("X Axis", &x_enable)) {
				if (x_enable)
					rotation.addAxis(AxisRotation::Axis::X);
				else
					rotation.removeAxis(AxisRotation::Axis::X);
			}
			if (x_enable) {
				f32 speed_x = rotation.getSpeedX();
				if (ImGui::DragFloat("X Speed", &speed_x, 0.05f, -FLT_MAX, FLT_MAX)) {
					rotation.setSpeedX(speed_x);
				}
			}

			//----------------------------------------------------------------------------------
			// Y Rotation
			//----------------------------------------------------------------------------------
			bool y_enable = rotation.hasAxis(AxisRotation::Axis::Y);

			if (ImGui::Checkbox("Y Axis", &y_enable)) {
				if (y_enable)
					rotation.addAxis(AxisRotation::Axis::Y);
				else
					rotation.removeAxis(AxisRotation::Axis::Y);
			}
			if (y_enable) {
				f32 speed_y = rotation.getSpeedY();
				if (ImGui::DragFloat("Y Speed", &speed_y, 0.05f, -FLT_MAX, FLT_MAX)) {
					rotation.setSpeedY(speed_y);
				}
			}

			//----------------------------------------------------------------------------------
			// Z Rotation
			//----------------------------------------------------------------------------------
			bool z_enable = rotation.hasAxis(AxisRotation::Axis::Z);

			if (ImGui::Checkbox("Z Axis", &z_enable)) {
				if (z_enable)
					rotation.addAxis(AxisRotation::Axis::Z);
				else
					rotation.removeAxis(AxisRotation::Axis::Z);
			}
			if (z_enable) {
				f32 speed_z = rotation.getSpeedZ();
				if (ImGui::DragFloat("Z Speed", &speed_z, 0.05f, -FLT_MAX, FLT_MAX)) {
					rotation.setSpeedZ(speed_z);
				}
			}
		};

		ui.registerUserComponent<AxisRotation>(axis_rotation);


		//----------------------------------------------------------------------------------
		// Axis Orbit
		//----------------------------------------------------------------------------------
		UserInterface::UserComponent axis_orbit;

		axis_orbit.name = "Axis Orbit";

		axis_orbit.getter = [](ecs::ECS& ecs, handle64 entity) -> typename ecs::IComponent* {
			return ecs.tryGet<AxisOrbit>(entity);
		};

		axis_orbit.adder = [](ecs::ECS& ecs, handle64 entity) -> void {
			ecs.add<AxisOrbit>(entity);
		};

		axis_orbit.details_renderer = [](ecs::IComponent& component) -> void {
			AxisOrbit& orbit = static_cast<AxisOrbit&>(component);
			auto axis = XMStore<f32_3>(orbit.getAxis());
			if (ImGui::InputFloat3("Axis", axis.data())) {
				orbit.setAxis(axis);
			}
		};

		ui.registerUserComponent<AxisOrbit>(axis_orbit);
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	handle64 scene_name_text;
};

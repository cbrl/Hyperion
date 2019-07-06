#include "picking_system.h"
#include "engine/engine.h"
#include "geometry/geometry.h"
#include "scene/events/core_events.h"



namespace render::systems {

PickingSystem::PickingSystem(Engine& engine)
	: engine(engine) {
}


void PickingSystem::registerCallbacks() {
	registerEventCallback(&PickingSystem::onGuiFocus);
}


void PickingSystem::onGuiFocus(const events::GuiFocusEvent& event) {
	if (event.mouse_focus || event.keyboard_focus)
		this->setActive(false);
	else
		this->setActive(true);
}


void PickingSystem::update() {
	const auto& input = engine.get().getInput();
	const i32_2 mouse = input.getMousePosition();

	// Can't select objects in relative mode or if the mouse wasn't clicked
	if (input.getMouseMode() == Mouse::MODE_RELATIVE ||
		input.isMouseLeftPressed() == false)
		return;
	
	// System will only check hits for the first camera found with a
	// viewport containing the mouse.
	bool found = false;

	getECS().forEach<PerspectiveCamera>([&](const PerspectiveCamera& camera) {
		if (found) return;

		const auto& viewport = camera.getViewport();
		if (viewport.contains(mouse[0], mouse[1])) {
			found = true;
			pick(camera);
		}
	});

	getECS().forEach<OrthographicCamera>([&](const OrthographicCamera& camera) {
		if (found) return;

		const auto& viewport = camera.getViewport();
		if (viewport.contains(mouse[0], mouse[1])) {
			found = true;
			pick(camera);
		}
	});
}


template<typename CameraT>
void PickingSystem::pick(const CameraT& camera) {
	auto* transform = this->getECS().tryGet<Transform>(camera.getOwner());
	if (not transform)
		return;
	
	// Mouse coords and viewport data
	const i32_2 mouse         = engine.get().getInput().getMousePosition();
	const u32_2 viewport_size = camera.getViewport().getSize();
	const f32   aspect        = camera.getViewport().getAspectRatio();

	// View and projection matrices
	const XMMATRIX world_to_view       = transform->getWorldToObjectMatrix();
	const XMMATRIX view_to_world       = XMMatrixInverse(nullptr, world_to_view);
	const XMMATRIX view_to_projection  = camera.getCameraToProjectionMatrix();
	const XMMATRIX world_to_projection = world_to_view * view_to_projection;
	
	XMFLOAT4X4 proj_data;
	XMStoreFloat4x4(&proj_data, view_to_projection);

	// NDC mouse coords
	const f32_2 p_ndc = {
		((2.0f * static_cast<f32>(mouse[0])) / static_cast<f32>(viewport_size[0])) - 1.0f,
		((-2.0f * static_cast<f32>(mouse[1])) / static_cast<f32>(viewport_size[1])) + 1.0f
	};

	// View-space mouse coords
	const f32_2 p_view = {
		p_ndc[0] / proj_data._11,
		p_ndc[1] / proj_data._22
	};

	// Ray origin and direction
	const XMVECTOR origin    = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	const XMVECTOR direction = XMVectorSet(p_view[0], p_view[1], 1.0f, 0.0f);

	// Check the scene for a hit
	castRay(origin, direction, view_to_world, world_to_projection);
}


void XM_CALLCONV PickingSystem::castRay(FXMVECTOR origin,
	                                    FXMVECTOR direction,
                                        CXMMATRIX view_to_world,
                                        CXMMATRIX world_to_projection) {
	auto& ecs = this->getECS();

	// For each model, if it's in the camera's view, cast a ray and see if it intersects the AABB.
	ecs.forEach<Transform, Model>([&](handle64 entity) {
		const auto& model     = ecs.get<Model>(entity);
		const auto& transform = ecs.get<Transform>(entity);

		const XMMATRIX model_to_world      = transform.getObjectToWorldMatrix();
		const XMMATRIX model_to_projection = model_to_world * world_to_projection;

		if (not Frustum(model_to_projection).contains(model.getAABB()))
			return;

		const XMMATRIX world_to_model = XMMatrixInverse(nullptr, model_to_world);
		const XMMATRIX view_to_model  = view_to_world * world_to_model;

		const XMVECTOR ray_origin    = XMVector3TransformCoord(origin, view_to_model);
		const XMVECTOR ray_direction = XMVector3Normalize(XMVector3TransformNormal(direction, view_to_model));

		if (intersects(ray_origin, ray_direction, model.getAABB()))
			sendEvent<events::EntitySelectedEvent>(model.getOwner());
	});
}

} //namespace render::systems
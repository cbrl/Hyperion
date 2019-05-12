#include "picking_system.h"
#include "engine/engine.h"
#include "geometry/frustum/frustum.h"
#include "scene/events/core_events.h"

bool intersects(const f32_3& origin, const f32_3& direction, const BoundingSphere& sphere) {
	const f32 a = std::pow(direction[0], 2) + std::pow(direction[1], 2) + std::pow(direction[2], 2);
	const f32 b = ((direction[0] * origin[0]) + (direction[1] * origin[1]) + (direction[2] * origin[2])) * 2.0f;
	const f32 c = std::pow(origin[0], 2) + std::pow(origin[1], 2) + std::pow(origin[2], 2) - std::pow(sphere.radius(), 2);

	const f32 discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0.0f)
		return false;

	return true;
}

bool intersects(XMVECTOR origin, XMVECTOR direction, const AABB& aabb) {
	f32 tmin = -std::numeric_limits<f32>::infinity();
	f32 tmax = std::numeric_limits<f32>::infinity();

	const XMVECTOR t1 = (aabb.min() - origin) / direction;
	const XMVECTOR t2 = (aabb.max() - origin) / direction;
	
	const f32_3 tmin1 = XMStore<f32_3>(XMVectorMin(t1, t2));
	const f32_3 tmax1 = XMStore<f32_3>(XMVectorMax(t1, t2));

	tmin = *std::max_element(tmin1.begin(), tmin1.end());
	tmax = *std::min_element(tmax1.begin(), tmax1.end());

	return tmax >= tmin;
}

bool intersects(const f32_3& origin, const f32_3& direction, const AABB& aabb) {
	const f32_3 min = XMStore<f32_3>(aabb.min());
	const f32_3 max = XMStore<f32_3>(aabb.max());
	
	f32 tmin = -std::numeric_limits<f32>::infinity();
	f32 tmax = std::numeric_limits<f32>::infinity();

	if (direction[0] != 0.0f) {
		const f32 tx1 = (min[0] - origin[0]) / direction[0];
		const f32 tx2 = (max[0] - origin[0]) / direction[0];

		tmin = std::max(tmin, std::min(tx1, tx2));
		tmax = std::min(tmax, std::max(tx1, tx2));
	}

	if (direction[1] != 0.0f) {
		const f32 ty1 = (min[1] - origin[1]) / direction[1];
		const f32 ty2 = (max[1] - origin[1]) / direction[1];

		tmin = std::max(tmin, std::min(ty1, ty2));
		tmax = std::min(tmax, std::max(ty1, ty2));
	}

	if (direction[2] != 0.0f) {
		const f32 tz1 = (min[2] - origin[2]) / direction[2];
		const f32 tz2 = (max[2] - origin[2]) / direction[2];

		tmin = std::max(tmin, std::min(tz1, tz2));
		tmax = std::min(tmax, std::max(tz1, tz2));
	}

	return tmax >= tmin;
}


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
	auto&       scene = engine.get().getScene();
	const auto& input = engine.get().getInput();

	// Can't select objects in relative mode or if the mouse wasn't clicked
	if (input.getMouseMode() == Mouse::MODE_RELATIVE ||
		input.isMouseLeftPressed() == false)
		return;
	
	const i32_2 mouse = input.getMousePosition();

	scene.forEach<PerspectiveCamera>([&](const PerspectiveCamera& camera) {
		const auto& viewport = camera.getViewport();

		if (viewport.contains(mouse[0], mouse[1])) {
			checkHit(camera);
		}
	});

	scene.forEach<OrthographicCamera>([&](const OrthographicCamera& camera) {
		const auto& viewport = camera.getViewport();

		if (viewport.contains(mouse[0], mouse[1])) {
			checkHit(camera);
		}
	});
}


template<typename CameraT>
void PickingSystem::checkHit(const CameraT& camera) {
	auto& scene = engine.get().getScene();
	auto* transform = camera.getOwner()->getComponent<Transform>();

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

	// For each model, if it's in the camera's view, cast a ray and see if it intersects the AABB.
	scene.forEach<Model>([&](const Model& model) {
		auto* transform = model.getOwner()->getComponent<Transform>();
		if (not transform)
			return;

		const auto model_to_world      = transform->getObjectToWorldMatrix();
		const auto model_to_projection = model_to_world * world_to_projection;

		if (not Frustum(model_to_projection).contains(model.getAABB()))
			return;

		const auto world_to_model = XMMatrixInverse(nullptr, model_to_world);
		const auto view_to_model  = view_to_world * world_to_model;

		const auto ray_origin    = XMVector3TransformCoord(origin, view_to_model);
		const auto ray_direction = XMVector3Normalize(XMVector3TransformNormal(direction, view_to_model));

		if (intersects(ray_origin, ray_direction, model.getAABB()))
			sendEvent<events::EntitySelectedEvent>(model.getOwner());
	});
}

} //namespace render::systems
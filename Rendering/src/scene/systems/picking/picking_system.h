#pragma once

#include "system/system.h"
#include "directxmath/directxmath.h"

class Engine;

namespace render {

namespace events { struct GuiFocusEvent; }
	
namespace systems {

class PickingSystem
	: public ecs::System<PickingSystem>
	, public ecs::EventSender
	, public ecs::EventListener {
public:
	PickingSystem(Engine& engine);
	PickingSystem(const PickingSystem&) = delete;
	PickingSystem(PickingSystem&&) = default;

	~PickingSystem() = default;

	PickingSystem& operator=(const PickingSystem&) = delete;
	PickingSystem& operator=(PickingSystem&&) = default;

	void update() override;

private:

	void registerCallbacks() override;

	void onGuiFocus(const events::GuiFocusEvent& event);

	template<typename CameraT>
	void pick(const CameraT& camera);

	void XM_CALLCONV castRay(FXMVECTOR origin,
	                         FXMVECTOR direction,
	                         CXMMATRIX view_to_world,
	                         CXMMATRIX world_to_projection);

	std::reference_wrapper<Engine> engine;
};

} //namespace systems
} //namespace render
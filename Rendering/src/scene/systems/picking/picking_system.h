#pragma once

#include "system/system.h"
#include "event/event_dispatcher.h"
#include "directxmath/directxmath.h"

class Engine;

namespace ecs {
	class ECS;
}

namespace render {

namespace events {
struct GuiFocusEvent;
}
	
namespace systems {

class PickingSystem : public ecs::System<PickingSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	PickingSystem(ecs::ECS& ecs, Engine& engine);
	PickingSystem(const PickingSystem&) = delete;
	PickingSystem(PickingSystem&&) = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~PickingSystem() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	PickingSystem& operator=(const PickingSystem&) = delete;
	PickingSystem& operator=(PickingSystem&&) = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	void onGuiFocus(const events::GuiFocusEvent& event);

	template<typename CameraT>
	void pick(const CameraT& camera);

	void XM_CALLCONV castRay(FXMVECTOR origin,
	                         FXMVECTOR direction,
	                         CXMMATRIX view_to_world,
	                         CXMMATRIX world_to_projection);

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<Engine> engine;
	ecs::UniqueDispatcherConnection gui_focus_connection;
};

} //namespace systems
} //namespace render
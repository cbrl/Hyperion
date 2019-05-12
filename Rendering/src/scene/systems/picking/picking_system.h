#pragma once

#include "system/system.h"

class Engine;

namespace render {

namespace events { class GuiFocusEvent; }
	
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
	void checkHit(const CameraT& camera);

	std::reference_wrapper<Engine> engine;
};

} //namespace systems
} //namespace render
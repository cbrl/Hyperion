#pragma once

#include "system/system.h"

namespace render::systems {

class HierarchySystem final : public ecs::System<HierarchySystem>, public ecs::EventListener {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	HierarchySystem() = default;
	HierarchySystem(const HierarchySystem&) = delete;
	HierarchySystem(HierarchySystem&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~HierarchySystem() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	HierarchySystem& operator=(const HierarchySystem&) = delete;
	HierarchySystem& operator=(HierarchySystem&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	void registerCallbacks() override;

	void onEntityDestroyed(const ecs::EntityDestroyed& event);
};

} //namespace render::systems
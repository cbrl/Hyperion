#pragma once

#include "system/system.h"

namespace ecs {
	class ECS;
}

namespace render {

class RenderingMgr;

namespace systems {

class ModelSystem final : public ecs::System<ModelSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ModelSystem(ecs::ECS& ecs, const RenderingMgr& rendering_mgr);
	ModelSystem(const ModelSystem&) = delete;
	ModelSystem(ModelSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~ModelSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ModelSystem& operator=(const ModelSystem&) = delete;
	ModelSystem& operator=(ModelSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<const RenderingMgr> rendering_mgr;
};

} //namespace systems
} //namespace render
module;

#include <functional>

#include "rendering_forward_decs.h"

export module systems.model_system;

import ecs;
import components.transform;
import components.model;

import rendering;


namespace render::systems {

export class ModelSystem final : public ecs::System {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ModelSystem(ecs::ECS& ecs, const RenderingMgr& rendering_mgr)
		: System(ecs)
		, rendering_mgr(rendering_mgr) {
	}

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
	void update() override {
		auto& ecs            = this->getECS();
		auto& device_context = rendering_mgr.get().getDeviceContext();

		ecs.forEach<Transform, Model>([&](handle64 entity) {
			const auto& transform = ecs.get<Transform>(entity);
			auto& model           = ecs.get<Model>(entity);

			// Update the model's buffer
			if (model.isActive()) {
				model.updateBuffer(device_context, transform.getObjectToWorldMatrix());
			}
		});
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<const RenderingMgr> rendering_mgr;
};

} //namespace render::systems

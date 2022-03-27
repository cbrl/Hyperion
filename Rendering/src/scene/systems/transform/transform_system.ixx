module;

#include "memory/handle/handle.h"

export module systems.transform_system;

import ecs;
import events.core_events;
import components.hierarchy;
import components.transform;


namespace render::systems {

export class TransformSystem final : public ecs::System {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TransformSystem(ecs::ECS& ecs)
		: System(ecs)
		, parent_changed_connection(ecs.getDispatcher<Hierarchy::ParentChangedEvent>().addCallback<&TransformSystem::onParentChanged>(this)) {
	}

	TransformSystem(const TransformSystem&) = delete;
	TransformSystem(TransformSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~TransformSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TransformSystem& operator=(const TransformSystem&) = delete;
	TransformSystem& operator=(TransformSystem&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override {
		auto& ecs = this->getECS();

		// Set update flags of child transforms
		ecs.forEach<Transform, Hierarchy>([this, &ecs](handle64 entity) {
			auto& transform = ecs.get<Transform>(entity);
			auto& hierarchy = ecs.get<Hierarchy>(entity);
		
			if (transform.needs_update) {
				hierarchy.forEachChildRecursive(ecs, [&ecs](handle64 child) {
					if (auto* child_transform = ecs.tryGet<Transform>(child)) {
						child_transform->setNeedsUpdate();
					}
				});
			}
		});

		// Update all transforms
		ecs.forEach<Transform>([this, &ecs](handle64 entity) {
			auto& transform = ecs.get<Transform>(entity);
			if (not transform.needs_update) {
				return;
			}

			const bool updated = updateWorld(transform);
			if (not updated) {
				return;
			}

			// Update children if their parent doesn't need an update
			if (auto* hierarchy = ecs.tryGet<Hierarchy>(entity)) {
				hierarchy->forEachChildRecursive(ecs, [this, &ecs](handle64 child) {
					if (auto* transform = ecs.tryGet<Transform>(child)) {
						updateWorld(*transform);
					}
				});
			}
		});
	}

private:

	void onParentChanged(const Hierarchy::ParentChangedEvent& event) {
		if (auto* transform = this->getECS().tryGet<Transform>(event.entity)) {
			transform->setNeedsUpdate();
		}
	} 

	bool updateWorld(Transform& transform) {
		auto& ecs = this->getECS();

		if (auto* hierarchy = ecs.tryGet<Hierarchy>(transform.getOwner());
			hierarchy && ecs.has<Transform>(hierarchy->getParent())) {

			auto& parent_transform = ecs.get<Transform>(hierarchy->getParent());

			if (parent_transform.needs_update)
				return false; //early return if the parent transform also needs an update

			auto m = parent_transform.getObjectToWorldMatrix();
			transform.update(&m);
		}
		else {
			transform.update();
		}

		transform.needs_update = false;
		return true;
	}

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ecs::UniqueDispatcherConnection parent_changed_connection;
};

} //namespace render::systems

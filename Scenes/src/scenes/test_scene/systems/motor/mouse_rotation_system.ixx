module;

#include "ecs.h"
#include "system/system.h"
#include "input.h"
#include "scene/components/transform/transform.h"

export module systems.motor.mouse_rotation;

import components.motor.mouse_rotation;


export class MouseRotationSystem final : public ecs::System<MouseRotationSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MouseRotationSystem(ecs::ECS& ecs, const Input& input)
		: System(ecs)
		, input(input) {
	}

	MouseRotationSystem(const MouseRotationSystem&) = delete;
	MouseRotationSystem(MouseRotationSystem&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~MouseRotationSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MouseRotationSystem& operator=(const MouseRotationSystem&) = delete;
	MouseRotationSystem& operator=(MouseRotationSystem&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override {
		auto& ecs = this->getECS();
		const i32_2 mouse_delta = input.get().getMouseDelta();

		ecs.forEach<Transform, MouseRotation>([&](handle64 entity) {
			auto& transform = ecs.get<Transform>(entity);
			const auto& rotation = ecs.get<MouseRotation>(entity);

			if (not rotation.isActive())
				return;

			const f32_2 max = rotation.getMaxRotation();
			f32_2 units{ 0.0f, 0.0f };

			// Set x/y rotation with mouse data
			units[0] = static_cast<f32>(mouse_delta[1]) * rotation.getSensitivity();
			units[1] = static_cast<f32>(mouse_delta[0]) * rotation.getSensitivity();

			// Rotate the camera
			if (units[0]) {
				transform.rotateXClamped(units[0], -max[0], max[0]);
			}
			if (units[1]) {
				transform.rotateYClamped(units[1], -max[1], max[1]);
			}
		});
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<const Input> input;
};

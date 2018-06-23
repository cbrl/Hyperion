#pragma once

#include "component/component.h"


class MouseRotation final : public Component<MouseRotation> {
public:
	MouseRotation() : rotate_sensitivity(0.1f) {}
	~MouseRotation() = default;

	[[nodiscard]]
	float sensitivity() const {
		return rotate_sensitivity;
	}

	void setSensitivity(float sensitivity) {
		rotate_sensitivity = sensitivity;
	}


private:
	float rotate_sensitivity;
};
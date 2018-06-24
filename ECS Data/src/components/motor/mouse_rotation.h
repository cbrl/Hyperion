#pragma once

#include "component/component.h"


class MouseRotation final : public Component<MouseRotation> {
public:
	MouseRotation() : sensitivity(0.1f) {}
	~MouseRotation() = default;

	[[nodiscard]]
	float getSensitivity() const {
		return sensitivity;
	}

	void setSensitivity(float value) {
		sensitivity = value;
	}


private:
	float sensitivity;
};
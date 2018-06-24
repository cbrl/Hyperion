#pragma once

#include "component/component.h"


class MouseRotation final : public Component<MouseRotation> {
public:
	MouseRotation() : sensitivity(0.1f) {}
	~MouseRotation() = default;

	[[nodiscard]]
	f32 getSensitivity() const {
		return sensitivity;
	}

	void setSensitivity(f32 value) {
		sensitivity = value;
	}


private:
	f32 sensitivity;
};
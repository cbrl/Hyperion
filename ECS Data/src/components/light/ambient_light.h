#pragma once

#include "component/component.h"
#include "buffer/buffers.h"


class AmbientLight final : public Component<AmbientLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AmbientLight()
		: color(0.0f, 0.0f, 0.0f, 1.0f) {
	}

	AmbientLight(const AmbientLight& light) = delete;
	AmbientLight(AmbientLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~AmbientLight() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	AmbientLight& operator=(const AmbientLight& light) = delete;
	AmbientLight& operator=(AmbientLight&& light) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	const vec4_f32& getColor() const {
		return color;
	}

	void setColor(const vec4_f32& ambient_color) {
		color = ambient_color;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	vec4_f32 color;
};
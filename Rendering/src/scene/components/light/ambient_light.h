#pragma once

#include "component/component.h"
#include "buffer/buffers.h"


class AmbientLight final : public ecs::Component<AmbientLight> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AmbientLight() noexcept = default;
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
	[[nodiscard]]
	const f32_3& getColor() const noexcept{
		return color;
	}

	[[nodiscard]]
	f32_3& getColor() noexcept {
		return color;
	}

	void setColor(const f32_3& ambient_color) noexcept {
		color = ambient_color;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32_3 color = {0.0f, 0.0f, 0.0f};
};

#pragma once

#include "datatypes/scalar_types.h"


enum class BlendStates : u8 {
	Opaque = 0,
	AlphaBlend,
	Additive,
	NonPremultiplied,
	StateCount
};

enum class DepthStencilStates : u8 {
	None = 0,
	LessEqR,
	LessEqRW,
	GreaterEqR,
	GreaterEqRW,
	StateCount
};

enum class RasterStates : u8 {
	CullNone = 0,
	CullClockwise,
	CullCounterClockwise,
	Wireframe,
	StateCount
};

enum class SamplerStates : u8 {
	PointClamp = 0,
	PointWrap,
	LinearClamp,
	LinearWrap,
	AnisotropicClamp,
	AnisotropicWrap,
	PCF,
	StateCount
};
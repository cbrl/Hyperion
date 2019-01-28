#pragma once

#include "datatypes/datatypes.h"


enum class AAType {
	None,
	MSAA_2x,
	MSAA_4x,
	MSAA_8x,
};

enum class RenderMode : u8 {
	Forward     = 0,
	ForwardPlus = 1,
	Deferred    = 2,
};

enum class LightingMode : u8 {
	Default              = 0,
	BRDF                 = 1,
	FalseColorFullbright = 2,
	FalseColorNormal     = 3,
	FalseColorDepth      = 4,
};

enum class RenderOptions : u8 {
	None           = 1,
	BoundingVolume = 1 << 1,
	Wireframe      = 1 << 2,
};

enum class BRDF : u8 {
	Lambert      = 0,
	BlinnPhong   = 1,
	CookTorrance = 2,
};
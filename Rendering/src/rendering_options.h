#pragma once

#include "datatypes/datatypes.h"


enum class AAType : u8 {
	None    = 0,
	MSAA_2x = 1,
	MSAA_4x = 2,
	MSAA_8x = 3,
};

enum class RenderMode : u8 {
	Forward     = 0,
	ForwardPlus = 1,
	Deferred    = 2,
};

enum class LightingMode : u8 {
	Default    = 0,
	BRDF       = 1,
	FalseColor = 2,
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

enum class FalseColor : u8 {
	Fullbright     = 0,
	TextureCoord   = 1,
	MaterialParams = 2,
	Metalness      = 3,
	Roughness      = 4,
	Normal         = 5,
	Depth          = 6,
	Static         = 7,
};
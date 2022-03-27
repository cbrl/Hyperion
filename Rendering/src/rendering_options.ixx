module;

#include "datatypes/scalar_types.h"

export module rendering.rendering_options;


export namespace render {

enum class AAType : u8 {
	None = 0,
	MSAA_2x,
	MSAA_4x,
	MSAA_8x,
};

enum class RenderMode : u8 {
	Forward = 0,
	ForwardPlus,
	Deferred,
	FalseColor,
};

enum class RenderOptions : u8 {
	None           = 1,
	BoundingVolume = 1 << 1,
	Wireframe      = 1 << 2,
};

enum class BRDF : u8 {
	Lambert = 0,
	BlinnPhong,
	Microfacet,
};

enum class FalseColor : u8 {
	Fullbright = 0,
	TextureCoord,
	MaterialParams,
	Metalness,
	Roughness,
	Normal,
	Depth,
	Static,
};

} //namespace render

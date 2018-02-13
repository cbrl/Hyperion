#include <VertexTypes.h>
#include "util\math\directxmath\extensions.h"

namespace DirectX {
	inline bool operator==(const VertexPositionNormal& first, const VertexPositionNormal& second) {
		return (first.position == second.position &&
				first.normal == second.normal);
	}


	inline bool operator!=(const VertexPositionNormal& first, const VertexPositionNormal& second) {
		return !(first == second);
	}


	inline bool operator==(const VertexPositionNormalTexture& first, const VertexPositionNormalTexture& second) {
		return (first.position == second.position &&
				first.normal == second.normal &&
				first.textureCoordinate == second.textureCoordinate);
	}


	inline bool operator!=(const VertexPositionNormalTexture& first, const VertexPositionNormalTexture& second) {
		return !(first == second);
	}
}
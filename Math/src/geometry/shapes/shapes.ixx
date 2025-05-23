module;

#include <algorithm>
#include <exception>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

#include "bezier.h"

export module math.geometry:shapes;

import math.directxmath;

export namespace render::shapes {

	template<typename VertexT>
	void ComputeCube(std::vector<VertexT>& vertices,
	                 std::vector<u32>& indices,
	                 f32  size     = 1,
	                 bool rhcoords = false,
	                 bool invertn  = false);

	template<typename VertexT>
	void ComputeBox(std::vector<VertexT>& vertices,
	                std::vector<u32>& indices,
	                const f32_3& size,
	                bool rhcoords = false,
	                bool invertn  = false);

	template<typename VertexT>
	void ComputeSphere(std::vector<VertexT>& vertices,
	                   std::vector<u32>& indices,
	                   f32    diameter     = 1,
	                   size_t tessellation = 16,
	                   bool   rhcoords     = false,
	                   bool   invertn      = false);

	template<typename VertexT>
	void ComputeGeoSphere(std::vector<VertexT>& vertices,
	                      std::vector<u32>& indices,
	                      f32    diameter     = 1,
	                      size_t tessellation = 3,
	                      bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeCylinder(std::vector<VertexT>& vertices,
	                     std::vector<u32>& indices,
	                     f32    diameter     = 1,
	                     f32    height       = 1,
	                     size_t tessellation = 32,
	                     bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeCone(std::vector<VertexT>& vertices,
	                 std::vector<u32>& indices,
	                 f32    diameter     = 1,
	                 f32    height       = 1,
	                 size_t tessellation = 32,
	                 bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeTorus(std::vector<VertexT>& vertices,
	                  std::vector<u32>& indices,
	                  f32    diameter     = 1,
	                  f32    thickness    = 0.333f,
	                  size_t tessellation = 32,
	                  bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeTetrahedron(std::vector<VertexT>& vertices,
	                        std::vector<u32>& indices,
	                        f32  size     = 1,
	                        bool rhcoords = false);
	
	template<typename VertexT>
	void ComputeOctahedron(std::vector<VertexT>& vertices,
	                       std::vector<u32>& indices,
	                       f32  size     = 1,
	                       bool rhcoords = false);
	
	template<typename VertexT>
	void ComputeDodecahedron(std::vector<VertexT>& vertices,
	                         std::vector<u32>& indices,
	                         f32  size     = 1,
	                         bool rhcoords = false);
	
	template<typename VertexT>
	void ComputeIcosahedron(std::vector<VertexT>& vertices,
	                        std::vector<u32>& indices,
	                        f32  size     = 1,
	                        bool rhcoords = false);

} //namespace render::shapes

#include "shapes.tpp"

//--------------------------------------------------------------------------------------
// File: Geometry.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#pragma once

#include "datatypes/datatypes.h"
#include "directx/vertex/vertex_types.h"


namespace Shapes {
	template<typename VertexT>
	void ComputeCube(std::vector<VertexT>& vertices,
	                 std::vector<u32>& indices,
	                 f32  size     = 1,
	                 bool rhcoords = false,
	                 bool invertn  = false);

	template<typename VertexT>
	void ComputeBox(std::vector<VertexT>& vertices,
	                std::vector<u32>& indices,
	                const vec3_f32& size,
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
}


#include "shapes.tpp"

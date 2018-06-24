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
#include "directx/vertex_types.h"


namespace Shapes {
	template<typename VertexT>
	void ComputeCube(vector<VertexT>& vertices,
	                 vector<u32>& indices,
	                 f32  size     = 1,
	                 bool rhcoords = false,
	                 bool invertn  = false);

	template<typename VertexT>
	void ComputeBox(vector<VertexT>& vertices,
	                vector<u32>& indices,
	                const f32_3& size,
	                bool rhcoords = false,
	                bool invertn  = false);

	template<typename VertexT>
	void ComputeSphere(vector<VertexT>& vertices,
	                   vector<u32>& indices,
	                   f32    diameter     = 1,
	                   size_t tessellation = 16,
	                   bool   rhcoords     = false,
	                   bool   invertn      = false);

	template<typename VertexT>
	void ComputeGeoSphere(vector<VertexT>& vertices,
	                      vector<u32>& indices,
	                      f32    diameter     = 1,
	                      size_t tessellation = 3,
	                      bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeCylinder(vector<VertexT>& vertices,
	                     vector<u32>& indices,
	                     f32    diameter     = 1,
	                     f32    height       = 1,
	                     size_t tessellation = 32,
	                     bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeCone(vector<VertexT>& vertices,
	                 vector<u32>& indices,
	                 f32    diameter     = 1,
	                 f32    height       = 1,
	                 size_t tessellation = 32,
	                 bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeTorus(vector<VertexT>& vertices,
	                  vector<u32>& indices,
	                  f32    diameter     = 1,
	                  f32    thickness    = 0.333f,
	                  size_t tessellation = 32,
	                  bool   rhcoords     = false);

	template<typename VertexT>
	void ComputeTetrahedron(vector<VertexT>& vertices,
	                        vector<u32>& indices,
	                        f32  size     = 1,
	                        bool rhcoords = false);
	
	template<typename VertexT>
	void ComputeOctahedron(vector<VertexT>& vertices,
	                       vector<u32>& indices,
	                       f32  size     = 1,
	                       bool rhcoords = false);
	
	template<typename VertexT>
	void ComputeDodecahedron(vector<VertexT>& vertices,
	                         vector<u32>& indices,
	                         f32  size     = 1,
	                         bool rhcoords = false);
	
	template<typename VertexT>
	void ComputeIcosahedron(vector<VertexT>& vertices,
	                        vector<u32>& indices,
	                        f32  size     = 1,
	                        bool rhcoords = false);
}


#include "shapes.tpp"

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

#include "util\datatypes\datatypes.h"

namespace Shapes
{
	template<typename VertexT>
	void ComputeCube(vector<VertexT>& vertices, vector<u32>& indices, float size = 1, bool rhcoords = false, bool invertn = false);
	template<typename VertexT>
    void ComputeBox(vector<VertexT>& vertices, vector<u32>& indices, const float3& size, bool rhcoords = false, bool invertn = false);
	template<typename VertexT>
    void ComputeSphere(vector<VertexT>& vertices, vector<u32>& indices, float diameter = 1, size_t tessellation = 16, bool rhcoords = false, bool invertn = false);
	template<typename VertexT>
    void ComputeGeoSphere(vector<VertexT>& vertices, vector<u32>& indices, float diameter = 1, size_t tessellation = 3, bool rhcoords = false);
	template<typename VertexT>
    void ComputeCylinder(vector<VertexT>& vertices, vector<u32>& indices, float diameter = 1, float height = 1, size_t tessellation = 32, bool rhcoords = false);
	template<typename VertexT>
	void ComputeCone(vector<VertexT>& vertices, vector<u32>& indices, float diameter = 1, float height = 1, size_t tessellation = 32, bool rhcoords = false);
	template<typename VertexT>
    void ComputeTorus(vector<VertexT>& vertices, vector<u32>& indices, float diameter = 1, float thickness = 0.333f, size_t tessellation = 32, bool rhcoords = false);
	template<typename VertexT>
    void ComputeTetrahedron(vector<VertexT>& vertices, vector<u32>& indices, float size = 1, bool rhcoords = false);
	template<typename VertexT>
    void ComputeOctahedron(vector<VertexT>& vertices, vector<u32>& indices, float size = 1, bool rhcoords = false);
	template<typename VertexT>
    void ComputeDodecahedron(vector<VertexT>& vertices, vector<u32>& indices, float size = 1, bool rhcoords = false);
	template<typename VertexT>
    void ComputeIcosahedron(vector<VertexT>& vertices, vector<u32>& indices, float size = 1, bool rhcoords = false);
}


#include "shapes.tpp"
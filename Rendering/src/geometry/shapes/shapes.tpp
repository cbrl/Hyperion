//--------------------------------------------------------------------------------------
// File: Geometry.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "bezier.h"

using namespace DirectX;


namespace Shapes {
	const f32 SQRT2 = 1.41421356237309504880f;
	const f32 SQRT3 = 1.73205080756887729352f;
	const f32 SQRT6 = 2.44948974278317809820f;

	inline void CheckIndexOverflow(size_t value) {
		// Use >=, not > comparison, because D3D level 10_x+ hardware does not support 0xFFFFFFFF index values.
		if (value >= UINT32_MAX)
			throw std::exception("Index value out of range: cannot tesselate primitive so finely");
	}


	// Collection types used when generating the geometry.
	inline void index_push_back(std::vector<u32>& indices, size_t value) {
		CheckIndexOverflow(value);
		indices.push_back(static_cast<u32>(value));
	}


	// Helper for flipping winding of geometric primitives for LH vs. RH coords
	template<typename VertexT>
	void ReverseWinding(std::vector<u32>& indices, std::vector<VertexT>& vertices) {
		assert((indices.size() % 3) == 0);
		for (auto it = indices.begin(); it != indices.end(); it += 3) {
			std::swap(*it, *(it + 2));
		}

		if constexpr (VertexT::hasTexture()) {
			for (auto it = vertices.begin(); it != vertices.end(); ++it) {
				vec2_f32 texCoord = it->texCoord;
				texCoord.x = 1.0f - texCoord.x;

				it->texCoord = texCoord;
			}
		}
	}


	// Helper for inverting normals of geometric primitives for 'inside' vs. 'outside' viewing
	template<typename VertexT>
	void InvertNormals(std::vector<VertexT>& vertices) {
		if constexpr (VertexT::hasNormal()) {
			for (auto it = vertices.begin(); it != vertices.end(); ++it) {
				vec3_f32 normal = it->normal;
				normal.x = -normal.x;
				normal.y = -normal.y;
				normal.z = -normal.z;

				it->normal = normal;
			}
		}
	}


	//--------------------------------------------------------------------------------------
	// Cube (aka a Hexahedron) or Box
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeCube(std::vector<VertexT>& vertices, std::vector<u32>& indices, f32 size, bool rhcoords, bool invertn) {
		ComputeBox(vertices, indices, vec3_f32(size, size, size), rhcoords, invertn);
	}

	template<typename VertexT>
	void ComputeBox(std::vector<VertexT>& vertices, std::vector<u32>& indices, const vec3_f32& size, bool rhcoords, bool invertn) {
		vertices.clear();
		indices.clear();

		// A box has six faces, each one pointing in a different direction.
		const int FaceCount = 6;

		static const XMVECTORF32 faceNormals[FaceCount] =
		{
			{{{ 0,  0,  1, 0}}},
			{{{ 0,  0, -1, 0}}},
			{{{ 1,  0,  0, 0}}},
			{{{-1,  0,  0, 0}}},
			{{{ 0,  1,  0, 0}}},
			{{{ 0, -1,  0, 0}}},
		};

		static const XMVECTORF32 texCoords[4] =
		{
			{{{1, 0, 0, 0}}},
			{{{1, 1, 0, 0}}},
			{{{0, 1, 0, 0}}},
			{{{0, 0, 0, 0}}},
		};

		XMVECTOR tsize = XMLoad(&size);
		tsize = XMVectorDivide(tsize, g_XMTwo);

		// Create each face in turn.
		for (int i = 0; i < FaceCount; i++) {
			const XMVECTOR normal = faceNormals[i];

			// Get two vectors perpendicular both to the face normal and to each other.
			const XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

			const XMVECTOR side1 = XMVector3Cross(normal, basis);
			const XMVECTOR side2 = XMVector3Cross(normal, side1);

			// Six indices (two triangles) per face.
			const size_t vbase = vertices.size();
			index_push_back(indices, vbase + 0);
			index_push_back(indices, vbase + 1);
			index_push_back(indices, vbase + 2);

			index_push_back(indices, vbase + 0);
			index_push_back(indices, vbase + 2);
			index_push_back(indices, vbase + 3);

			// Four vertices per face.
			vertices.resize(vertices.size() + 4);

			XMStore(&vertices.rbegin()[3].position, (normal - side1 - side2) * tsize);
			XMStore(&vertices.rbegin()[2].position, (normal - side1 + side2) * tsize);
			XMStore(&vertices.rbegin()[1].position, (normal + side1 + side2) * tsize);
			XMStore(&vertices.rbegin()[0].position, (normal + side1 - side2) * tsize);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[3].normal, normal);
				XMStore(&vertices.rbegin()[2].normal, normal);
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[3].texCoord, texCoords[0]);
				XMStore(&vertices.rbegin()[2].texCoord, texCoords[1]);
				XMStore(&vertices.rbegin()[1].texCoord, texCoords[2]);
				XMStore(&vertices.rbegin()[0].texCoord, texCoords[3]);
			}
		}

		// Build RH above
		if (!rhcoords)
			ReverseWinding(indices, vertices);

		if (invertn)
			InvertNormals(vertices);
	}


	//--------------------------------------------------------------------------------------
	// Sphere
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeSphere(std::vector<VertexT>& vertices,
	                   std::vector<u32>& indices,
	                   f32 diameter,
	                   size_t tessellation,
	                   bool rhcoords,
	                   bool invertn) {
		vertices.clear();
		indices.clear();

		if (tessellation < 3)
			throw std::out_of_range("tesselation parameter out of range");

		const size_t verticalSegments = tessellation;
		const size_t horizontalSegments = tessellation * 2;

		const f32 radius = diameter / 2;

		// Create rings of vertices at progressively higher latitudes.
		for (size_t i = 0; i <= verticalSegments; i++) {
			const f32 v = 1 - static_cast<f32>(i) / verticalSegments;

			const f32 latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
			f32 dy, dxz;

			XMScalarSinCos(&dy, &dxz, latitude);

			// Create a single ring of vertices at this latitude.
			for (size_t j = 0; j <= horizontalSegments; j++) {
				const f32 u = static_cast<f32>(j) / horizontalSegments;

				const f32 longitude = j * XM_2PI / horizontalSegments;
				f32 dx, dz;

				XMScalarSinCos(&dx, &dz, longitude);

				dx *= dxz;
				dz *= dxz;

				const XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
				const XMVECTOR texCoord = XMVectorSet(u, v, 0, 0);

				vertices.push_back(VertexT());
				XMStore(&vertices.back().position, normal * radius);
				if constexpr (VertexT::hasNormal())
					XMStore(&vertices.back().normal, normal);
				if constexpr (VertexT::hasTexture())
					XMStore(&vertices.back().texCoord, texCoord);
			}
		}

		// Fill the index buffer with triangles joining each pair of latitude rings.
		const size_t stride = horizontalSegments + 1;

		for (size_t i = 0; i < verticalSegments; i++) {
			for (size_t j = 0; j <= horizontalSegments; j++) {
				const size_t nextI = i + 1;
				const size_t nextJ = (j + 1) % stride;

				index_push_back(indices, i * stride + j);
				index_push_back(indices, nextI * stride + j);
				index_push_back(indices, i * stride + nextJ);

				index_push_back(indices, i * stride + nextJ);
				index_push_back(indices, nextI * stride + j);
				index_push_back(indices, nextI * stride + nextJ);
			}
		}

		// Build RH above
		if (!rhcoords)
			ReverseWinding(indices, vertices);

		if (invertn)
			InvertNormals(vertices);
	}


	//--------------------------------------------------------------------------------------
	// Geodesic sphere
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeGeoSphere(std::vector<VertexT>& vertices,
	                      std::vector<u32>& indices,
	                      f32 diameter,
	                      size_t tessellation,
	                      bool rhcoords) {
		vertices.clear();
		indices.clear();

		// An undirected edge between two vertices, represented by a pair of indexes into a vertex array.
		// Becuse this edge is undirected, (a,b) is the same as (b,a).
		typedef std::pair<u32, u32> UndirectedEdge;

		// Makes an undirected edge. Rather than overloading comparison operators to give us the (a,b)==(b,a) property,
		// we'll just ensure that the larger of the two goes first. This'll simplify things greatly.
		auto makeUndirectedEdge = [](u32 a, u32 b) {
			return std::make_pair(std::max(a, b), std::min(a, b));
		};

		// Key: an edge
		// Value: the index of the vertex which lies midway between the two vertices pointed to by the key value
		// This std::map is used to avoid duplicating vertices when subdividing triangles along edges.
		typedef std::map<UndirectedEdge, u32> EdgeSubdivisionMap;


		static const vec3_f32 OctahedronVertices[] =
		{
			// when looking down the negative z-axis (into the screen)
			vec3_f32(0, 1, 0), // 0 top
			vec3_f32(0, 0, -1), // 1 front
			vec3_f32(1, 0, 0), // 2 right
			vec3_f32(0, 0, 1), // 3 back
			vec3_f32(-1, 0, 0), // 4 left
			vec3_f32(0, -1, 0), // 5 bottom
		};
		static const u32 OctahedronIndices[] =
		{
			0, 1, 2, // top front-right face
			0, 2, 3, // top back-right face
			0, 3, 4, // top back-left face
			0, 4, 1, // top front-left face
			5, 1, 4, // bottom front-left face
			5, 4, 3, // bottom back-left face
			5, 3, 2, // bottom back-right face
			5, 2, 1, // bottom front-right face
		};

		const f32 radius = diameter / 2.0f;

		// Start with an octahedron; copy the data into the vertex/index collection.

		std::vector<vec3_f32> vertexPositions(std::begin(OctahedronVertices), std::end(OctahedronVertices));

		indices.insert(indices.begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));

		// We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
		// about to go on, these values aren't ever going to change because the vertices don't move around in the array.
		// We'll need these values later on to fix the singularities that show up at the poles.
		const u32 northPoleIndex = 0;
		const u32 southPoleIndex = 5;

		for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision) {
			assert(indices.size() % 3 == 0); // sanity

			// We use this to keep track of which edges have already been subdivided.
			EdgeSubdivisionMap subdividedEdges;

			// The new index collection after subdivision.
			std::vector<u32> newIndices;

			const size_t triangleCount = indices.size() / 3;
			for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle) {
				// For each edge on this triangle, create a new vertex in the middle of that edge.
				// The winding order of the triangles we output are the same as the winding order of the inputs.

				// Indices of the vertices making up this triangle
				u32 iv0 = indices[iTriangle * 3 + 0];
				u32 iv1 = indices[iTriangle * 3 + 1];
				u32 iv2 = indices[iTriangle * 3 + 2];

				// Get the new vertices
				vec3_f32 v01; // vertex on the midpoint of v0 and v1
				vec3_f32 v12; // ditto v1 and v2
				vec3_f32 v20; // ditto v2 and v0
				u32 iv01; // index of v01
				u32 iv12; // index of v12
				u32 iv20; // index of v20

				// Function that, when given the index of two vertices, creates a new vertex at the midpoint of those vertices.
				auto divideEdge = [&](u32 i0, u32 i1, vec3_f32& outVertex, u32& outIndex) {
					const UndirectedEdge edge = makeUndirectedEdge(i0, i1);

					// Check to see if we've already generated this vertex
					const auto it = subdividedEdges.find(edge);
					if (it != subdividedEdges.end()) {
						// We've already generated this vertex before
						outIndex = it->second; // the index of this vertex
						outVertex = vertexPositions[outIndex]; // and the vertex itself
					}
					else {
						// Haven't generated this vertex before: so add it now

						// outVertex = (vertices[i0] + vertices[i1]) / 2
						XMStore(
						              &outVertex,
						              XMVectorScale(
						                            XMVectorAdd(XMLoad(&vertexPositions[i0]),
						                                        XMLoad(&vertexPositions[i1])),
						                            0.5f
						                           )
						             );

						outIndex = static_cast<u32>(vertexPositions.size());
						CheckIndexOverflow(outIndex);
						vertexPositions.push_back(outVertex);

						// Now add it to the std::map.
						subdividedEdges.insert(std::make_pair(edge, outIndex));
					}
				};

				// Add/get new vertices and their indices
				divideEdge(iv0, iv1, v01, iv01);
				divideEdge(iv1, iv2, v12, iv12);
				divideEdge(iv0, iv2, v20, iv20);

				// Add the new indices. We have four new triangles from our original one:
				//        v0
				//        o
				//       /a\
				//  v20 o---o v01
				//     /b\c/d\
				// v2 o---o---o v1
				//       v12
				const u32 indicesToAdd[] =
				{
					iv0, iv01, iv20, // a
					iv20, iv12, iv2, // b
					iv20, iv01, iv12, // c
					iv01, iv1, iv12, // d
				};
				newIndices.insert(newIndices.end(), std::begin(indicesToAdd), std::end(indicesToAdd));
			}

			indices = std::move(newIndices);
		}

		// Now that we've completed subdivision, fill in the final vertex collection
		vertices.reserve(vertexPositions.size());
		for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it) {
			auto vertexValue = *it;

			const auto normal = XMVector3Normalize(XMLoad(&vertexValue));
			const auto pos = XMVectorScale(normal, radius);

			vec3_f32 normalFloat3;
			XMStore(&normalFloat3, normal);

			// calculate texture coordinates for this vertex
			const f32 longitude = atan2(normalFloat3.x, -normalFloat3.z);
			const f32 latitude = acos(normalFloat3.y);

			const f32 u = longitude / XM_2PI + 0.5f;
			const f32 v = latitude / XM_PI;

			const auto texcoord = XMVectorSet(1.0f - u, v, 0.0f, 0.0f);

			vertices.push_back(VertexT());
			XMStore(&vertices.back().position, pos);
			if constexpr (VertexT::hasNormal())
				XMStore(&vertices.back().normal, normal);
			if constexpr (VertexT::hasTexture())
				XMStore(&vertices.back().texCoord, texcoord);
		}

		// There are a couple of fixes to do. One is a texture coordinate wraparound fixup. At some point, there will be
		// a set of triangles somewhere in the mesh with texture coordinates such that the wraparound across 0.0/1.0
		// occurs across that triangle. Eg. when the left hand side of the triangle has a U coordinate of 0.98 and the
		// right hand side has a U coordinate of 0.0. The intent is that such a triangle should render with a U of 0.98 to
		// 1.0, not 0.98 to 0.0. If we don't do this fixup, there will be a visible seam across one side of the sphere.
		// 
		// Luckily this is relatively easy to fix. There is a straight edge which runs down the prime meridian of the
		// completed sphere. If you imagine the vertices along that edge, they circumscribe a semicircular arc starting at
		// y=1 and ending at y=-1, and sweeping across the range of z=0 to z=1. x stays zero. It's along this edge that we
		// need to duplicate our vertices - and provide the correct texture coordinates.
		if constexpr (VertexT::hasTexture()) {
			const size_t preFixupVertexCount = vertices.size();
			for (size_t i = 0; i < preFixupVertexCount; ++i) {
				// This vertex is on the prime meridian if position.x and texcoord.u are both zero (allowing for small epsilon).
				const bool isOnPrimeMeridian = XMVector2NearEqual(
				                                            XMVectorSet(vertices[i].position.x,
				                                                        vertices[i].texCoord.x,
				                                                        0.0f,
				                                                        0.0f),
				                                            XMVectorZero(),
				                                            XMVectorSplatEpsilon());

				if (isOnPrimeMeridian) {
					const size_t newIndex = vertices.size(); // the index of this vertex that we're about to add
					CheckIndexOverflow(newIndex);

					// copy this vertex, correct the texture coordinate, and add the vertex
					VertexT v = vertices[i];
					v.texCoord.x = 1.0f;
					vertices.push_back(v);

					// Now find all the triangles which contain this vertex and update them if necessary
					for (size_t j = 0; j < indices.size(); j += 3) {
						u32* triIndex0 = &indices[j + 0];
						u32* triIndex1 = &indices[j + 1];
						u32* triIndex2 = &indices[j + 2];

						if (*triIndex0 == i) {
							// nothing; just keep going
						}
						else if (*triIndex1 == i) {
							std::swap(triIndex0, triIndex1); // swap the pointers (not the values)
						}
						else if (*triIndex2 == i) {
							std::swap(triIndex0, triIndex2); // swap the pointers (not the values)
						}
						else {
							// this triangle doesn't use the vertex we're interested in
							continue;
						}

						// If we got to this point then triIndex0 is the pointer to the index to the vertex we're looking at
						assert(*triIndex0 == i);
						assert(*triIndex1 != i && *triIndex2 != i); // assume no degenerate triangles

						const VertexT& v0 = vertices[*triIndex0];
						const VertexT& v1 = vertices[*triIndex1];
						const VertexT& v2 = vertices[*triIndex2];

						// check the other two vertices to see if we might need to fix this triangle

						if (abs(v0.texCoord.x - v1.texCoord.x) > 0.5f ||
						    abs(v0.texCoord.x - v2.texCoord.x) > 0.5f) {
							// yep; replace the specified index to point to the new, corrected vertex
							*triIndex0 = static_cast<u32>(newIndex);
						}
					}
				}
			}
		}

		// And one last fix we need to do: the poles. A common use-case of a sphere mesh is to std::map a rectangular texture onto
		// it. If that happens, then the poles become singularities which std::map the entire top and bottom rows of the texture
		// onto a single point. In general there's no real way to do that right. But to match the behavior of non-geodesic
		// spheres, we need to duplicate the pole vertex for every triangle that uses it. This will introduce seams near the
		// poles, but reduce stretching.
		auto fixPole = [&](size_t poleIndex) {
			if constexpr (VertexT::hasTexture()) {
				auto poleVertex = vertices[poleIndex];
				bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex

				for (size_t i = 0; i < indices.size(); i += 3) {
					// These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
					// entry in the index array which represents the pole index, and the other two pointers point to the other
					// two indices making up this triangle.
					u32* pPoleIndex;
					u32* pOtherIndex0;
					u32* pOtherIndex1;
					if (indices[i + 0] == poleIndex) {
						pPoleIndex = &indices[i + 0];
						pOtherIndex0 = &indices[i + 1];
						pOtherIndex1 = &indices[i + 2];
					}
					else if (indices[i + 1] == poleIndex) {
						pPoleIndex = &indices[i + 1];
						pOtherIndex0 = &indices[i + 2];
						pOtherIndex1 = &indices[i + 0];
					}
					else if (indices[i + 2] == poleIndex) {
						pPoleIndex = &indices[i + 2];
						pOtherIndex0 = &indices[i + 0];
						pOtherIndex1 = &indices[i + 1];
					}
					else {
						continue;
					}

					const auto& otherVertex0 = vertices[*pOtherIndex0];
					const auto& otherVertex1 = vertices[*pOtherIndex1];

					// Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
					VertexT newPoleVertex = poleVertex;
					newPoleVertex.texCoord.x = (otherVertex0.texCoord.x + otherVertex1.texCoord.x) / 2;
					newPoleVertex.texCoord.y = poleVertex.texCoord.y;

					if (!overwrittenPoleVertex) {
						vertices[poleIndex] = newPoleVertex;
						overwrittenPoleVertex = true;
					}
					else {
						CheckIndexOverflow(vertices.size());

						*pPoleIndex = static_cast<u32>(vertices.size());
						vertices.push_back(newPoleVertex);
					}
				}
			}
		};


		fixPole(northPoleIndex);
		fixPole(southPoleIndex);

		// Build RH above
		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}


	//--------------------------------------------------------------------------------------
	// Cylinder / Cone
	//--------------------------------------------------------------------------------------
	namespace {
		// Helper computes a point on a unit circle, aligned to the x/z plane and centered on the origin.
		XMVECTOR GetCircleVector(size_t i, size_t tessellation) {
			const f32 angle = i * XM_2PI / tessellation;
			f32 dx, dz;

			XMScalarSinCos(&dx, &dz, angle);

			const XMVECTORF32 v = {{{dx, 0, dz, 0}}};
			return v;
		}

		XMVECTOR GetCircleTangent(size_t i, size_t tessellation) {
			const f32 angle = (i * XM_2PI / tessellation) + XM_PIDIV2;
			f32 dx, dz;

			XMScalarSinCos(&dx, &dz, angle);

			const XMVECTORF32 v = {{{dx, 0, dz, 0}}};
			return v;
		}


		// Helper creates a triangle fan to close the end of a cylinder / cone
		template<typename VertexT>
		void CreateCylinderCap(std::vector<VertexT>& vertices,
		                       std::vector<u32>& indices,
		                       size_t tessellation,
		                       f32 height,
		                       f32 radius,
		                       bool isTop) {
			// Create cap indices.
			for (size_t i = 0; i < tessellation - 2; i++) {
				size_t i1 = (i + 1) % tessellation;
				size_t i2 = (i + 2) % tessellation;

				if (isTop) {
					std::swap(i1, i2);
				}

				const size_t vbase = vertices.size();
				index_push_back(indices, vbase);
				index_push_back(indices, vbase + i1);
				index_push_back(indices, vbase + i2);
			}

			// Which end of the cylinder is this?
			XMVECTOR normal = g_XMIdentityR1;
			XMVECTOR textureScale = g_XMNegativeOneHalf;

			if (!isTop) {
				normal = -normal;
				textureScale *= g_XMNegateX;
			}

			// Create cap vertices.
			for (size_t i = 0; i < tessellation; i++) {
				const XMVECTOR circleVector = GetCircleVector(i, tessellation);

				const XMVECTOR position = (circleVector * radius) + (normal * height);

				const XMVECTOR texCoord = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector),
				                                        textureScale,
				                                        g_XMOneHalf);

				vertices.push_back(VertexT());
				XMStore(&vertices.back().position, position);
				if constexpr (VertexT::hasNormal())
					XMStore(&vertices.back().normal, normal);
				if constexpr (VertexT::hasTexture())
					XMStore(&vertices.back().texCoord, texCoord);
			}
		}
	}


	template<typename VertexT>
	void ComputeCylinder(std::vector<VertexT>& vertices,
	                     std::vector<u32>& indices,
	                     f32 diameter,
	                     f32 height,
	                     size_t tessellation,
	                     bool rhcoords) {
		vertices.clear();
		indices.clear();

		if (tessellation < 3)
			throw std::out_of_range("tesselation parameter out of range");

		height /= 2;

		const XMVECTOR topOffset = g_XMIdentityR1 * height;

		f32 radius = diameter / 2;
		const size_t stride = tessellation + 1;

		// Create a ring of triangles around the outside of the cylinder.
		for (size_t i = 0; i <= tessellation; i++) {
			const XMVECTOR normal = GetCircleVector(i, tessellation);

			const XMVECTOR sideOffset = normal * radius;

			f32 u = static_cast<f32>(i) / tessellation;

			const XMVECTOR texCoord = XMLoad(&u);

			vertices.resize(vertices.size() + 2);

			XMStore(&vertices.rbegin()[1].position, sideOffset + topOffset);
			XMStore(&vertices.rbegin()[0].position, sideOffset - topOffset);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[1].texCoord, texCoord);
				XMStore(&vertices.rbegin()[0].texCoord, texCoord + g_XMIdentityR1);
			}


			index_push_back(indices, i * 2);
			index_push_back(indices, (i * 2 + 2) % (stride * 2));
			index_push_back(indices, i * 2 + 1);

			index_push_back(indices, i * 2 + 1);
			index_push_back(indices, (i * 2 + 2) % (stride * 2));
			index_push_back(indices, (i * 2 + 3) % (stride * 2));
		}

		// Create flat triangle fan caps to seal the top and bottom.
		CreateCylinderCap(vertices, indices, tessellation, height, radius, true);
		CreateCylinderCap(vertices, indices, tessellation, height, radius, false);

		// Build RH above
		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}


	// Creates a cone primitive.
	template<typename VertexT>
	void ComputeCone(std::vector<VertexT>& vertices,
	                 std::vector<u32>& indices,
	                 f32 diameter,
	                 f32 height,
	                 size_t tessellation,
	                 bool rhcoords) {
		vertices.clear();
		indices.clear();

		if (tessellation < 3)
			throw std::out_of_range("tesselation parameter out of range");

		height /= 2;

		const XMVECTOR topOffset = g_XMIdentityR1 * height;

		f32 radius = diameter / 2;
		const size_t stride = tessellation + 1;

		// Create a ring of triangles around the outside of the cone.
		for (size_t i = 0; i <= tessellation; i++) {
			const XMVECTOR circlevec = GetCircleVector(i, tessellation);

			const XMVECTOR sideOffset = circlevec * radius;

			f32 u = static_cast<f32>(i) / tessellation;

			const XMVECTOR texCoord = XMLoad(&u);

			const XMVECTOR pt = sideOffset - topOffset;

			XMVECTOR normal = XMVector3Cross(GetCircleTangent(i, tessellation), topOffset - pt);
			normal = XMVector3Normalize(normal);

			// Duplicate the top vertex for distinct normals
			vertices.resize(vertices.size() + 2);

			XMStore(&vertices.rbegin()[1].position, topOffset);
			XMStore(&vertices.rbegin()[0].position, pt);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[1].texCoord, g_XMZero);
				XMStore(&vertices.rbegin()[0].texCoord, texCoord + g_XMIdentityR1);
			}

			index_push_back(indices, i * 2);
			index_push_back(indices, (i * 2 + 3) % (stride * 2));
			index_push_back(indices, (i * 2 + 1) % (stride * 2));
		}

		// Create flat triangle fan caps to seal the bottom.
		CreateCylinderCap(vertices, indices, tessellation, height, radius, false);

		// Build RH above
		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}


	//--------------------------------------------------------------------------------------
	// Torus
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeTorus(std::vector<VertexT>& vertices,
	                  std::vector<u32>& indices,
	                  f32 diameter,
	                  f32 thickness,
	                  size_t tessellation,
	                  bool rhcoords) {
		vertices.clear();
		indices.clear();

		if (tessellation < 3)
			throw std::out_of_range("tesselation parameter out of range");

		const size_t stride = tessellation + 1;

		// First we loop around the main ring of the torus.
		for (size_t i = 0; i <= tessellation; i++) {
			const f32 u = static_cast<f32>(i) / tessellation;

			const f32 outerAngle = i * XM_2PI / tessellation - XM_PIDIV2;

			// Create a transform matrix that will align geometry to
			// slice perpendicularly though the current ring position.
			const XMMATRIX transform = XMMatrixTranslation(diameter / 2, 0, 0) * XMMatrixRotationY(outerAngle);

			// Now we loop along the other axis, around the side of the tube.
			for (size_t j = 0; j <= tessellation; j++) {
				const f32 v = 1 - static_cast<f32>(j) / tessellation;

				const f32 innerAngle = j * XM_2PI / tessellation + XM_PI;
				f32 dx, dy;

				XMScalarSinCos(&dy, &dx, innerAngle);

				// Create a vertex.
				XMVECTOR normal = XMVectorSet(dx, dy, 0, 0);
				XMVECTOR position = normal * thickness / 2;
				const XMVECTOR texCoord = XMVectorSet(u, v, 0, 0);

				position = XMVector3Transform(position, transform);
				normal = XMVector3TransformNormal(normal, transform);

				vertices.push_back(VertexT());
				XMStore(&vertices.back().position, position);
				if constexpr (VertexT::hasNormal())
					XMStore(&vertices.back().normal, normal);
				if constexpr (VertexT::hasTexture())
					XMStore(&vertices.back().texCoord, texCoord);

				// And create indices for two triangles.
				const size_t nextI = (i + 1) % stride;
				const size_t nextJ = (j + 1) % stride;

				index_push_back(indices, i * stride + j);
				index_push_back(indices, i * stride + nextJ);
				index_push_back(indices, nextI * stride + j);

				index_push_back(indices, i * stride + nextJ);
				index_push_back(indices, nextI * stride + nextJ);
				index_push_back(indices, nextI * stride + j);
			}
		}

		// Build RH above
		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}


	//--------------------------------------------------------------------------------------
	// Tetrahedron
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeTetrahedron(std::vector<VertexT>& vertices, std::vector<u32>& indices, f32 size, bool rhcoords) {
		vertices.clear();
		indices.clear();

		static const XMVECTORF32 verts[4] =
		{
			{{{0.f, 0.f, 1.f, 0}}},
			{{{2.f * SQRT2 / 3.f, 0.f, -1.f / 3.f, 0}}},
			{{{-SQRT2 / 3.f, SQRT6 / 3.f, -1.f / 3.f, 0}}},
			{{{-SQRT2 / 3.f, -SQRT6 / 3.f, -1.f / 3.f, 0}}}
		};

		static const uint32_t faces[4 * 3] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 3, 1,
			1, 3, 2,
		};

		for (size_t j = 0; j < _countof(faces); j += 3) {
			const uint32_t v0 = faces[j];
			const uint32_t v1 = faces[j + 1];
			const uint32_t v2 = faces[j + 2];

			XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
			                                 verts[v2].v - verts[v0].v);
			normal = XMVector3Normalize(normal);

			const size_t base = vertices.size();
			index_push_back(indices, base);
			index_push_back(indices, base + 1);
			index_push_back(indices, base + 2);

			// Duplicate vertices to use face normals
			vertices.resize(vertices.size() + 3);

			XMVECTOR position = XMVectorScale(verts[v0], size);
			XMStore(&vertices.rbegin()[2].position, position);

			position = XMVectorScale(verts[v1], size);
			XMStore(&vertices.rbegin()[1].position, position);

			position = XMVectorScale(verts[v2], size);
			XMStore(&vertices.rbegin()[0].position, position);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[2].normal, normal);
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[2].texCoord, g_XMZero);
				XMStore(&vertices.rbegin()[1].texCoord, g_XMIdentityR0);
				XMStore(&vertices.rbegin()[0].texCoord, g_XMIdentityR1);
			}
		}

		// Built LH above
		if (rhcoords)
			ReverseWinding(indices, vertices);

		assert(vertices.size() == 4 * 3);
		assert(indices.size() == 4 * 3);
	}


	//--------------------------------------------------------------------------------------
	// Octahedron
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeOctahedron(std::vector<VertexT>& vertices, std::vector<u32>& indices, f32 size, bool rhcoords) {
		vertices.clear();
		indices.clear();

		static const XMVECTORF32 verts[6] =
		{
			{{{ 1,  0,  0, 0}}},
			{{{-1,  0,  0, 0}}},
			{{{ 0,  1,  0, 0}}},
			{{{ 0, -1,  0, 0}}},
			{{{ 0,  0,  1, 0}}},
			{{{ 0,  0, -1, 0}}}
		};

		static const uint32_t faces[8 * 3] =
		{
			4, 0, 2,
			4, 2, 1,
			4, 1, 3,
			4, 3, 0,
			5, 2, 0,
			5, 1, 2,
			5, 3, 1,
			5, 0, 3
		};

		for (size_t j = 0; j < _countof(faces); j += 3) {
			const uint32_t v0 = faces[j];
			const uint32_t v1 = faces[j + 1];
			const uint32_t v2 = faces[j + 2];

			XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
			                                 verts[v2].v - verts[v0].v);
			normal = XMVector3Normalize(normal);

			const size_t base = vertices.size();
			index_push_back(indices, base);
			index_push_back(indices, base + 1);
			index_push_back(indices, base + 2);

			// Duplicate vertices to use face normals
			vertices.resize(vertices.size() + 3);

			XMVECTOR position = XMVectorScale(verts[v0], size);
			XMStore(&vertices.rbegin()[2].position, position);

			position = XMVectorScale(verts[v1], size);
			XMStore(&vertices.rbegin()[1].position, position);

			position = XMVectorScale(verts[v2], size);
			XMStore(&vertices.rbegin()[0].position, position);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[2].normal, normal);
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[2].texCoord, g_XMZero);
				XMStore(&vertices.rbegin()[1].texCoord, g_XMIdentityR0);
				XMStore(&vertices.rbegin()[0].texCoord, g_XMIdentityR1);
			}
		}

		// Built LH above
		if (rhcoords)
			ReverseWinding(indices, vertices);

		assert(vertices.size() == 8 * 3);
		assert(indices.size() == 8 * 3);
	}


	//--------------------------------------------------------------------------------------
	// Dodecahedron
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeDodecahedron(std::vector<VertexT>& vertices, std::vector<u32>& indices, f32 size, bool rhcoords) {
		vertices.clear();
		indices.clear();

		static const f32 a = 1.f / SQRT3;
		static const f32 b = 0.356822089773089931942f; // sqrt( ( 3 - sqrt(5) ) / 6 )
		static const f32 c = 0.934172358962715696451f; // sqrt( ( 3 + sqrt(5) ) / 6 );

		static const XMVECTORF32 verts[20] =
		{
			{{{ a,  a,  a, 0}}},
			{{{ a,  a, -a, 0}}},
			{{{ a, -a,  a, 0}}},
			{{{ a, -a, -a, 0}}},
			{{{-a,  a,  a, 0}}},
			{{{-a,  a, -a, 0}}},
			{{{-a, -a,  a, 0}}},
			{{{-a, -a, -a, 0}}},
			{{{ b,  c,  0, 0}}},
			{{{-b,  c,  0, 0}}},
			{{{ b, -c,  0, 0}}},
			{{{-b, -c,  0, 0}}},
			{{{ c,  0,  b, 0}}},
			{{{ c,  0, -b, 0}}},
			{{{-c,  0,  b, 0}}},
			{{{-c,  0, -b, 0}}},
			{{{ 0,  b,  c, 0}}},
			{{{ 0, -b,  c, 0}}},
			{{{ 0,  b, -c, 0}}},
			{{{ 0, -b, -c, 0}}}
		};

		static const uint32_t faces[12 * 5] =
		{
			 0,   8,   9,   4,  16,
			 0,  16,  17,   2,  12,
			12,   2,  10,   3,  13,
			 9,   5,  15,  14,   4,
			 3,  19,  18,   1,  13,
			 7,  11,   6,  14,  15,
			 0,  12,  13,   1,   8,
			 8,   1,  18,   5,   9,
			16,   4,  14,   6,  17,
			 6,  11,  10,   2,  17,
			 7,  15,   5,  18,  19,
			 7,  19,   3,  10,  11,
		};

		static const XMVECTORF32 texCoords[5] =
		{
			{{{0.654508f,  0.0244717f, 0, 0}}},
			{{{0.0954915f, 0.206107f,  0, 0}}},
			{{{0.0954915f, 0.793893f,  0, 0}}},
			{{{0.654508f,  0.975528f,  0, 0}}},
			{{{1.f,        0.5f,       0, 0}}}
		};

		static const uint32_t textureIndex[12][5] =
		{
			{0, 1, 2, 3, 4},
			{2, 3, 4, 0, 1},
			{4, 0, 1, 2, 3},
			{1, 2, 3, 4, 0},
			{2, 3, 4, 0, 1},
			{0, 1, 2, 3, 4},
			{1, 2, 3, 4, 0},
			{4, 0, 1, 2, 3},
			{4, 0, 1, 2, 3},
			{1, 2, 3, 4, 0},
			{0, 1, 2, 3, 4},
			{2, 3, 4, 0, 1},
		};

		size_t t = 0;
		for (size_t j = 0; j < _countof(faces); j += 5, ++t) {
			const uint32_t v0 = faces[j];
			const uint32_t v1 = faces[j + 1];
			const uint32_t v2 = faces[j + 2];
			const uint32_t v3 = faces[j + 3];
			const uint32_t v4 = faces[j + 4];

			XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
			                                 verts[v2].v - verts[v0].v);
			normal = XMVector3Normalize(normal);

			const size_t base = vertices.size();

			index_push_back(indices, base);
			index_push_back(indices, base + 1);
			index_push_back(indices, base + 2);

			index_push_back(indices, base);
			index_push_back(indices, base + 2);
			index_push_back(indices, base + 3);

			index_push_back(indices, base);
			index_push_back(indices, base + 3);
			index_push_back(indices, base + 4);

			// Duplicate vertices to use face normals
			vertices.resize(vertices.size() + 5);

			XMVECTOR position = XMVectorScale(verts[v0], size);
			XMStore(&vertices.rbegin()[4].position, position);

			position = XMVectorScale(verts[v1], size);
			XMStore(&vertices.rbegin()[3].position, position);

			position = XMVectorScale(verts[v2], size);
			XMStore(&vertices.rbegin()[2].position, position);

			position = XMVectorScale(verts[v3], size);
			XMStore(&vertices.rbegin()[1].position, position);

			position = XMVectorScale(verts[v4], size);
			XMStore(&vertices.rbegin()[0].position, position);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[4].normal, normal);
				XMStore(&vertices.rbegin()[3].normal, normal);
				XMStore(&vertices.rbegin()[2].normal, normal);
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[4].texCoord, texCoords[textureIndex[t][0]]);
				XMStore(&vertices.rbegin()[3].texCoord, texCoords[textureIndex[t][1]]);
				XMStore(&vertices.rbegin()[2].texCoord, texCoords[textureIndex[t][2]]);
				XMStore(&vertices.rbegin()[1].texCoord, texCoords[textureIndex[t][3]]);
				XMStore(&vertices.rbegin()[0].texCoord, texCoords[textureIndex[t][4]]);
			}
		}

		// Built LH above
		if (rhcoords)
			ReverseWinding(indices, vertices);

		assert(vertices.size() == 12 * 5);
		assert(indices.size() == 12 * 3 * 3);
	}


	//--------------------------------------------------------------------------------------
	// Icosahedron
	//--------------------------------------------------------------------------------------
	template<typename VertexT>
	void ComputeIcosahedron(std::vector<VertexT>& vertices, std::vector<u32>& indices, f32 size, bool rhcoords) {
		vertices.clear();
		indices.clear();

		static const f32 t = 1.618033988749894848205f; // (1 + sqrt(5)) / 2
		static const f32 t2 = 1.519544995837552493271f; // sqrt( 1 + sqr( (1 + sqrt(5)) / 2 ) )

		static const XMVECTORF32 verts[12] =
		{
			{{{ t / t2,    1.f / t2,  0,        0}}},
			{{{-t / t2,    1.f / t2,  0,        0}}},
			{{{ t / t2,   -1.f / t2,  0,        0}}},
			{{{-t / t2,   -1.f / t2,  0,        0}}},
			{{{ 1.f / t2,  0,         t / t2,   0}}},
			{{{ 1.f / t2,  0,        -t / t2,   0}}},
			{{{-1.f / t2,  0,         t / t2,   0}}},
			{{{-1.f / t2,  0,        -t / t2,   0}}},
			{{{ 0,         t / t2,    1.f / t2, 0}}},
			{{{ 0,        -t / t2,    1.f / t2, 0}}},
			{{{ 0,         t / t2,   -1.f / t2, 0}}},
			{{{ 0,        -t / t2,   -1.f / t2, 0}}}
		};

		static const uint32_t faces[20 * 3] =
		{
			 0,  8,  4,
			 0,  5, 10,
			 2,  4,  9,
			 2, 11,  5,
			 1,  6,  8,
			 1, 10,  7,
			 3,  9,  6,
			 3,  7, 11,
			 0, 10,  8,
			 1,  8, 10,
			 2,  9, 11,
			 3, 11,  9,
			 4,  2,  0,
			 5,  0,  2,
			 6,  1,  3,
			 7,  3,  1,
			 8,  6,  4,
			 9,  4,  6,
			10,  5,  7,
			11,  7,  5
		};

		for (size_t j = 0; j < _countof(faces); j += 3) {
			const uint32_t v0 = faces[j];
			const uint32_t v1 = faces[j + 1];
			const uint32_t v2 = faces[j + 2];

			XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
			                                 verts[v2].v - verts[v0].v);
			normal = XMVector3Normalize(normal);

			const size_t base = vertices.size();
			index_push_back(indices, base);
			index_push_back(indices, base + 1);
			index_push_back(indices, base + 2);

			// Duplicate vertices to use face normals
			vertices.resize(vertices.size() + 3);

			XMVECTOR position = XMVectorScale(verts[v0], size);
			XMStore(&vertices.rbegin()[2].position, position);

			position = XMVectorScale(verts[v1], size);
			XMStore(&vertices.rbegin()[1].position, position);

			position = XMVectorScale(verts[v2], size);
			XMStore(&vertices.rbegin()[0].position, position);

			if constexpr (VertexT::hasNormal()) {
				XMStore(&vertices.rbegin()[2].normal, normal);
				XMStore(&vertices.rbegin()[1].normal, normal);
				XMStore(&vertices.rbegin()[0].normal, normal);
			}

			if constexpr (VertexT::hasTexture()) {
				XMStore(&vertices.rbegin()[2].texCoord, g_XMZero);
				XMStore(&vertices.rbegin()[1].texCoord, g_XMIdentityR0);
				XMStore(&vertices.rbegin()[0].texCoord, g_XMIdentityR1);
			}
		}

		// Built LH above
		if (rhcoords)
			ReverseWinding(indices, vertices);

		assert(vertices.size() == 20 * 3);
		assert(indices.size() == 20 * 3);
	}
}

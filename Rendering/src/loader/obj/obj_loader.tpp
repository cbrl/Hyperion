#include <algorithm>
#include "io/io.h"
#include "log/log.h"
#include "directx/directx_math.h"
#include "loader/obj/obj_tokens.h"
#include "loader/material_loader.h"
#include "resource/resource_mgr.h"


template<typename VertexT>
ObjLoader<VertexT>::ObjLoader(ResourceMgr& resource_mgr)
	: resource_mgr(resource_mgr) {
}


template<typename VertexT>
ModelOutput<VertexT> ObjLoader<VertexT>::load(const fs::path& file, bool right_hand_coords) {

	// Set the coordinate system
	rh_coord = right_hand_coords;

	// Load the model
	readFile(file);

	// Sometimes a group is defined at the top of the file, then an extra one
	// before the first vertex. If that happened, then remove the extra.
	if (groups.size() > 1) {
		if (groups[1].index_start == 0) {
			groups.erase(groups.begin() + 1);
		}
	}

	// Load the materials
	auto path = getFilePath();
	path.replace_filename(mat_lib);
	loadMaterials(path);

	// Create and return the model
	return ModelOutput<VertexT>(file.filename().string(), vertices, indices, materials, groups);
}


template<typename VertexT>
void ObjLoader<VertexT>::readLine() {

	const auto token = readToken<std::string_view>();

	if (token[0] == ObjTokens::comment) {
		return;
	}

	// Vertex
	if (token == ObjTokens::vertex) {
		vec3_f32 position;
		position.x = readToken<f32>();
		position.y = readToken<f32>();
		position.z = readToken<f32>();

		if (rh_coord) {
			position.z *= -1.0f;
		}

		vertex_positions.push_back(position);
	}

	// Normal
	else if (token == ObjTokens::normal) {
		vec3_f32 normal;
		normal.x = readToken<f32>();
		normal.y = readToken<f32>();
		normal.z = readToken<f32>();

		if (rh_coord) {
			normal.z *= -1.0f;
		}

		vertex_normals.push_back(normal);
	}

	// Texture
	else if (token == ObjTokens::texture) {
		vec2_f32 tex_coord;
		tex_coord.x = readToken<f32>();
		tex_coord.y = readToken<f32>();

		if (rh_coord) {
			tex_coord.y = 1.0f - tex_coord.y;
		}

		vertex_texCoords.push_back(tex_coord);
	}

	// Group
	else if (token == ObjTokens::group) {
		groups.emplace_back();
		groups.back().name = readToken<std::string>();
		groups.back().index_start = static_cast<u32>(indices.size());
	}

	// Face
	else if (token == ObjTokens::face) {
		readFace();
	}

	// Material Library
	else if (token == ObjTokens::mtl_library) {
		mat_lib = readToken<std::string>();
	}

	// Group Material
	else if (token == ObjTokens::use_mtl) {
		group_mat_names[static_cast<u32>(groups.size()) - 1] = readToken<std::string>();
	}

	// Unknown Token
	else {
		Logger::log(LogLevel::warn, "{}:{} Unknown obj token: {}", getFilePath().string(), getLineNumber(), token);
		return;
	}


	readUnusedTokens();
}


template<typename VertexT>
void ObjLoader<VertexT>::loadMaterials(const fs::path& mat_file) {

	MaterialLoader::load(resource_mgr, mat_file, materials);

	// Set the group's material to the index value of its
	// material in the material vector.
	for (u32 i = 0; i < groups.size(); ++i) {
		for (u32 j = 0; j < materials.size(); ++j) {
			if (group_mat_names[i] == materials[j].name) {
				groups[i].material_index = j;
			}
		}
	}
}


template<typename VertexT>
void ObjLoader<VertexT>::readFace() {

	std::vector<vec3_u32> face_def;

	// Split the line into separate vertex definitions
	std::vector<std::string_view> vert_strings;
	while (hasTokens()) {
		vert_strings.push_back(readToken<std::string_view>());
	}

	for (const auto& vert_string : vert_strings) {

		// Split the vertex definition into separate parts
		std::vector<std::string_view> vert_parts = Split(vert_string, "/");

		vec3_u32 vertex{0, 0, 0};

		// Determine the vertex type
		switch (vert_parts.size()) {
			// Position
			case 1: {
				vertex.x = *StrTo<u32>(vert_parts[0]);
				face_def.push_back(vertex);
				break;
			}

			// Position/Texture
			case 2: {
				vertex.x = *StrTo<u32>(vert_parts[0]);
				vertex.y = *StrTo<u32>(vert_parts[1]);
				face_def.push_back(vertex);
				break;
			}

			case 3: {
				// Position//Normal
				if (vert_parts[1].empty()) {
					vertex.x = *StrTo<u32>(vert_parts[0]);
					vertex.z = *StrTo<u32>(vert_parts[2]);
					face_def.push_back(vertex);
				}
				// Position/Texture/Normal
				else {
					vertex.x = *StrTo<u32>(vert_parts[0]);
					vertex.y = *StrTo<u32>(vert_parts[1]);
					vertex.z = *StrTo<u32>(vert_parts[2]);
					face_def.push_back(vertex);
				}
				break;
			}

			default:
				break;
		}
	}


	// If no group has been defined, then create one manually
	if (groups.empty()) {
		groups.emplace_back();
		groups.back().index_start = static_cast<u32>(indices.size());
	}

	// Triangulate the face if there were more than 3 points
	if (face_def.size() > 3) {
		triangulate(face_def);
	}

	// Add the indices and any new vertices
	for (auto& vertex : face_def) {
		const auto it = index_map.find(vertex);
		if (it != index_map.cend()) {
			indices.push_back(it->second);
		}
		else {
			const auto index = static_cast<u32>(vertices.size());
			indices.push_back(index);
			vertices.push_back(createVertex(vertex));
			index_map[vertex] = index;
		}
	}
}


template<typename VertexT>
VertexT ObjLoader<VertexT>::createVertex(vec3_u32 vert_def) {
	
	VertexT out;

	// Position
	if (vert_def.x)
		out.position = vertex_positions[vert_def.x - 1];

	// Texture
	if constexpr(VertexT::hasTexture()) {
		if (vert_def.y)
			out.texCoord = vertex_texCoords[vert_def.y - 1];
	}

	// Normal
	if constexpr(VertexT::hasNormal()) {
		if (vert_def.z)
			out.normal = vertex_normals[vert_def.z - 1];
	}

	return out;
}


// Converts the input face into triangles
template<typename VertexT>
void ObjLoader<VertexT>::triangulate(std::vector<vec3_u32>& face_def) {

	// Create the vertices from the input face
	std::vector<VertexT> input_verts;
	std::vector<VertexT> working_verts;
	for (const auto& vert_def : face_def) {
		auto vertex = createVertex(vert_def);
		input_verts.push_back(vertex);
		working_verts.push_back(vertex);
	}

	// Indices of the triangle vertices (index is relative to input vector)
	std::vector<u32> tri_indices;

	while (true) {
		for (size_t i = 0; i < working_verts.size(); ++i) {

			// Previous, current, and next vertices
			vec3_f32 prev;
			vec3_f32 curr = working_verts[i].position;
			vec3_f32 next;

			// Previous vertex value
			if (i == 0) {
				prev = working_verts.back().position;
			}
			else {
				prev = working_verts[i - 1].position;
			}

			// Next vertex value
			if (i == working_verts.size() - 1) {
				next = working_verts[0].position;
			}
			else {
				next = working_verts[i + 1].position;
			}


			// Handle a 3 vertex case then break
			if (working_verts.size() == 3) {
				for (u32 j = 0; j < working_verts.size(); ++j) {
					if (input_verts[j].position == prev) tri_indices.push_back(j);
					if (input_verts[j].position == curr) tri_indices.push_back(j);
					if (input_verts[j].position == next) tri_indices.push_back(j);
				}

				working_verts.clear();
				break;
			}

			// Handle a 4 vertex case then break
			if (working_verts.size() == 4) {
				// Create a triangle
				for (u32 j = 0; j < input_verts.size(); ++j) {
					if (input_verts[j].position == prev) tri_indices.push_back(j);
					if (input_verts[j].position == curr) tri_indices.push_back(j);
					if (input_verts[j].position == next) tri_indices.push_back(j);
				}

				// Find the vertex that isn't prev, curr, or next
				vec3_f32 temp;
				for (size_t j = 0; j < working_verts.size(); ++j) {
					if (working_verts[j].position != prev &&
					    working_verts[j].position != curr &&
					    working_verts[j].position != next) {

						temp = working_verts[j].position;
						break;
					}
				}

				// Create a triangle
				for (u32 j = 0; j < input_verts.size(); ++j) {
					if (input_verts[j].position == prev) tri_indices.push_back(j);
					if (input_verts[j].position == next) tri_indices.push_back(j);
					if (input_verts[j].position == temp) tri_indices.push_back(j);
				}

				working_verts.clear();
				break;
			}


			// Ensure that the vertex isn't an interior vertex
			const vec3_f32 v1   = prev - curr;
			const vec3_f32 v2   = next - curr;
			const XMVECTOR vec1 = XMLoad(&v1);
			const XMVECTOR vec2 = XMLoad(&v2);

			f32 angle = XMVectorGetX(XMVector3AngleBetweenVectors(vec1, vec2));
			angle     = XMConvertToDegrees(angle);

			if (angle <= 0 || angle >= 180) {
				continue;
			}


			// Ensure that no vertices are inside the triangle
			bool in_triangle = false;
			for (size_t j = 0; j < input_verts.size(); ++j) {
				if (PointInTriangle(prev, curr, next, input_verts[j].position)
				    && input_verts[j].position != prev
				    && input_verts[j].position != curr
				    && input_verts[j].position != next) {
					in_triangle = true;
					break;
				}
			}
			if (in_triangle) {
				continue;
			}

			// Create a triangle from previous, current, and next vertices
			for (u32 j = 0; j < input_verts.size(); ++j) {
				if (input_verts[j].position == prev)
					tri_indices.push_back(j);
				if (input_verts[j].position == curr)
					tri_indices.push_back(j);
				if (input_verts[j].position == next)
					tri_indices.push_back(j);
			}

			// Delete current vertex from the list
			for (size_t j = 0; j < working_verts.size(); ++j) {
				if (working_verts[j].position == curr) {
					working_verts.erase(working_verts.begin() + j);
					break;
				}
			}

			// Reset i
			i = -1;
		}

		// Break if no triangles were created, or there are no more vertices to process
		if (indices.empty() || working_verts.empty()) {
			break;
		}
	}


	// Rebuild the face definition in terms of triangles
	std::vector<vec3_u32> temp_face = face_def;
	face_def.clear();
	for (const auto& index : tri_indices) {
		face_def.push_back(temp_face[index]);
	}
}

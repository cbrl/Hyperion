#include <algorithm>
#include "loader/obj/obj_tokens.h"
#include "io/io.h"
#include "directx/directx_math.h"


template<typename VertexT>
void OBJLoader<VertexT>::reset() {
	// Reset the local variables
	rh_coord    = false;
	group_count = 0;
	mtl_count   = 0;

	// Clear vectors
	vertices.clear();
	indices.clear();
	vertex_positions.clear();
	vertex_normals.clear();
	vertex_texCoords.clear();
	mat_lib.clear();
	materials.clear();
	groups.clear();
	group_mat_names.clear();
}


template<typename VertexT>
[[nodiscard]]
ModelOutput<VertexT> OBJLoader<VertexT>::load(ResourceMgr& resource_mgr,
                                              const wstring& filename,
                                              bool right_hand_coords) {

	// Make sure the file exists first
	if (!fs::exists(filename)) {
		Logger::log(LogLevel::err, "File does not exist: {}", wstr2str(filename));
		throw std::exception("OBJ file not found");
	}

	// Set the coordinate system
	rh_coord = right_hand_coords;


	// Get the folder that the file is in
	const wstring folder = GetParentPath(filename);

	// Load the model
	loadModel(filename);

	// Load the materials
	loadMaterials(folder);


	// Create the materials
	vector<Material> mtl_vector;
	for (u32 i = 0; i < mtl_count; ++i) {
		Material mtl;

		mtl.name = wstr2str(materials[i].name);

		if (!materials[i].map_Kd.empty())
			mtl.map_diffuse = resource_mgr.getOrCreate<Texture>(folder + materials[i].map_Kd);
		else
			mtl.has_texture = false;

		if (!materials[i].map_Ka.empty())
			mtl.map_alpha = resource_mgr.getOrCreate<Texture>(folder + materials[i].map_Ka);

		if (!materials[i].map_Ks.empty())
			mtl.map_specular = resource_mgr.getOrCreate<Texture>(folder + materials[i].map_Ks);

		if (!materials[i].map_Ns.empty())
			mtl.map_spec_highlight = resource_mgr.getOrCreate<Texture>(folder + materials[i].map_Ns);

		if (!materials[i].map_d.empty())
			mtl.map_alpha = resource_mgr.getOrCreate<Texture>(folder + materials[i].map_d);

		if (!materials[i].map_bump.empty())
			mtl.map_bump = resource_mgr.getOrCreate<Texture>(folder + materials[i].map_bump);

		mtl.ambient         = materials[i].Ka;
		mtl.diffuse         = materials[i].Kd;
		mtl.specular        = materials[i].Ks;
		mtl.emissive        = materials[i].Ke;
		mtl.dissolve        = materials[i].d;
		mtl.optical_density = materials[i].Ni;
		mtl.illum           = materials[i].illum;
		mtl.transparent     = materials[i].transparency;

		mtl_vector.push_back(std::move(mtl));
	}

	// Create the model
	string name = wstr2str(GetFilename(filename));
	ModelOutput<VertexT> out(name, vertices, indices, mtl_vector, groups);


	// Reset the obj loader
	reset();


	return out;
}


template<typename VertexT>
void OBJLoader<VertexT>::loadModel(wstring filename) {

	// Open the file
	wifstream file(filename);
	if (file.fail()) {
		Logger::log(LogLevel::err, "Could not open obj file: {}", wstr2str(filename));
		throw std::exception("Could not open obj file");
	}


	//----------------------------------------------------------------------------------
	// Read file contents
	//----------------------------------------------------------------------------------

	wstring line;
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		if (line[0] == ObjTokens::comment || line.empty()) {
			continue;
		}

		// Copy the first token from the line
		wstring token = line.substr(0, line.find_first_of(L' '));

		// Remove the token from the line
		line = line.substr(token.size());
		line = TrimWhiteSpace(line);

		// Create a stringstream to easily read data into variables
		wstringstream stream(line);


		// Vertex
		if (token == ObjTokens::vertex) {
			vec3_f32 position;
			stream >> position.x >> position.y >> position.z;

			if (rh_coord) {
				position.z *= -1.0f;
			}

			vertex_positions.push_back(position);
		}

		// Normal
		else if (token == ObjTokens::normal) {
			vec3_f32 normal;
			stream >> normal.x >> normal.y >> normal.z;

			if (rh_coord) {
				normal.z *= -1.0f;
			}

			vertex_normals.push_back(normal);
		}

		// Texture
		else if (token == ObjTokens::texture) {
			vec2_f32 texCoord;
			stream >> texCoord.x >> texCoord.y;

			if (rh_coord) {
				texCoord.y = 1.0f - texCoord.y;
			}

			vertex_texCoords.push_back(texCoord);
		}

		// Group
		else if (token == ObjTokens::group) {
			wstring name;
			stream >> name;

			groups.push_back(Group());
			groups.back().name        = wstr2str(name);
			groups.back().index_start = static_cast<u32>(indices.size());

			++group_count;
		}

		// Face
		else if (token == ObjTokens::face) {
			readFace(line);
		}

		// Material Library
		if (token == ObjTokens::mtl_library) {
			mat_lib = TrimWhiteSpace(line);
		}

		// Group Material
		else if (token == ObjTokens::use_mtl) {
			group_mat_names[group_count - 1] = TrimWhiteSpace(line);
		}
	}

	// Close the OBJ file
	file.close();


	// Sometimes a group is defined at the top of the file, then an extra one
	// before the first vertex. If that happened, then remove the extra.
	if (groups.size() > 1) {
		if (groups[1].index_start == 0) {
			groups.erase(groups.begin() + 1);
			--group_count;
		}
	}
}


template<typename VertexT>
void OBJLoader<VertexT>::loadMaterials(wstring folder) {

	// Open the material file
	wifstream file(folder + mat_lib);
	if (file.fail()) {
		Logger::log(LogLevel::err, "Could not open mtl file: {}", wstr2str(folder + mat_lib));
		throw std::exception("Could not open mtl file");
	}


	//----------------------------------------------------------------------------------
	// Read file contents
	//----------------------------------------------------------------------------------

	wstring line;
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		if (line[0] == ObjTokens::comment || line.empty()) {
			continue;
		}

		// Copy the first token from the line
		wstring token = line.substr(0, line.find_first_of(L' '));

		// Remove the token from the line
		line = line.substr(token.size());
		line = TrimWhiteSpace(line);

		// Create a stringstream to easily read data into variables
		wstringstream stream(line);


		// New Material
		if (token == ObjTokens::new_mtl) {
			OBJMaterial temp;
			temp.name = line;
			materials.push_back(temp);

			++mtl_count;
		}

		// Diffuse Color
		else if (token == ObjTokens::diffuse_color) {
			stream >> materials[mtl_count - 1].Kd.x;
			stream >> materials[mtl_count - 1].Kd.y;
			stream >> materials[mtl_count - 1].Kd.z;
			materials[mtl_count - 1].Kd.w = 1.0f;
		}

		// Ambient Color
		else if (token == ObjTokens::ambient_color) {
			stream >> materials[mtl_count - 1].Ka.x;
			stream >> materials[mtl_count - 1].Ka.y;
			stream >> materials[mtl_count - 1].Ka.z;
			materials[mtl_count - 1].Ka.w = 1.0f;
		}

		// Specular Color
		else if (token == ObjTokens::specular_color) {
			stream >> materials[mtl_count - 1].Ks.x;
			stream >> materials[mtl_count - 1].Ks.y;
			stream >> materials[mtl_count - 1].Ks.z;
		}

		// Emissive Color
		else if (token == ObjTokens::emissive_color) {
			stream >> materials[mtl_count - 1].Ke.x;
			stream >> materials[mtl_count - 1].Ke.y;
			stream >> materials[mtl_count - 1].Ke.z;
			materials[mtl_count - 1].Ke.w = 1.0f;
		}

		// Specular Expononet
		else if (token == ObjTokens::specular_exponent) {
			stream >> materials[mtl_count - 1].Ks.w;
		}

		// Optical Density
		else if (token == ObjTokens::optical_density) {
			stream >> materials[mtl_count - 1].Ni;
		}

		// Dissolve (transparency)
		else if (token == ObjTokens::transparency) {
			readTransparency(line, false);
		}

		// Dissolve (transparency inverse)
		else if (token == ObjTokens::transparency_inv) {
			readTransparency(line, true);
		}

		// Illumination
		else if (token == ObjTokens::illumination_model) {
			stream >> materials[mtl_count - 1].illum;
		}

		// Diffuse Map
		else if (token == ObjTokens::diffuse_color_map) {
			materials[mtl_count - 1].map_Kd = line;
		}

		// Alpha Map
		else if (token == ObjTokens::alpha_texture_map) {
			materials[mtl_count - 1].map_d        = line;
			materials[mtl_count - 1].transparency = true;
		}

		// Ambient Map
		else if (token == ObjTokens::ambient_color_map) {
			materials[mtl_count - 1].map_Ka = line;
		}

		// Specular Map
		else if (token == ObjTokens::specular_color_map) {
			materials[mtl_count - 1].map_Ks = line;
		}

		// Specular Highlight Map
		else if (token == ObjTokens::spec_highlight_map) {
			materials[mtl_count - 1].map_Ns = line;
		}

		// Bump Map
		else if (token == ObjTokens::bump_map || token == ObjTokens::bump_map2) {
			materials[mtl_count - 1].map_bump = line;
		}
	}

	// Close the material file
	file.close();


	// Set the group's material to the index value of its
	// material in the material vector.
	for (u32 i = 0; i < group_count; ++i) {

		// Use the first material if the group has no material assigned
		if (group_mat_names[i].empty()) {
			group_mat_names[i] = materials[0].name;
		}

		for (u32 j = 0; j < materials.size(); ++j) {
			if (group_mat_names[i] == materials[j].name) {
				groups[i].material_index = j;
			}
		}
	}
}


template<typename VertexT>
void OBJLoader<VertexT>::readTransparency(wstring& line, bool inverse) {
	wstringstream stream(line);

	f32 transparency;
	stream >> transparency;

	if (inverse) {
		transparency = 1.0f - transparency;
	}

	materials[mtl_count - 1].d = transparency;

	if (transparency > 0.0f) {
		materials[mtl_count - 1].transparency = true;
	}
}


template<typename VertexT>
void OBJLoader<VertexT>::readFace(wstring& line) {

	vector<vec3_u32> face_def;

	// Split the line into separate vertex definitions
	vector<wstring> vert_strings = Split(line, L" ");

	for (size_t i = 0; i < vert_strings.size(); ++i) {

		// Split the vertex definition into separate parts
		vector<wstring> vert_parts = Split(vert_strings[i], L"/");

		vec3_u32 vertex{ 0, 0, 0 };

		// Determine the vertex type
		switch (vert_parts.size()) {
			// Position
			case 1:
				vertex.x = stoi(vert_parts[0]);
				face_def.push_back(vertex);
				break;

				// Position/Texture
			case 2:
				vertex.x = stoi(vert_parts[0]);
				vertex.y = stoi(vert_parts[1]);
				face_def.push_back(vertex);
				break;

			case 3:
				// Position//Normal
				if (vert_parts[1].empty()) {
					vertex.x = stoi(vert_parts[0]);
					vertex.z = stoi(vert_parts[2]);
					face_def.push_back(vertex);
				}
				// Position/Texture/Normal
				else {
					vertex.x = stoi(vert_parts[0]);
					vertex.y = stoi(vert_parts[1]);
					vertex.z = stoi(vert_parts[2]);
					face_def.push_back(vertex);
				}
				break;

			default:
				break;
		}
	}


	// If no group has been defined, then create one manually
	if (group_count == 0) {
		groups.emplace_back();
		groups.back().index_start = static_cast<u32>(indices.size());
		++group_count;
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
			u32 index = static_cast<u32>(vertices.size());
			indices.push_back(index);
			vertices.push_back(createVertex(vertex));
			index_map[vertex] = index;
		}
	}
}


template<typename VertexT>
VertexT OBJLoader<VertexT>::createVertex(vec3_u32 vert_def) {
	
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
void OBJLoader<VertexT>::triangulate(vector<vec3_u32>& face_def) {

	// Create the vertices from the input faces
	vector<VertexT> in_verts;
	vector<VertexT> v_verts;
	for (const auto& vert_def : face_def) {
		auto vertex = createVertex(vert_def);
		in_verts.push_back(vertex);
		v_verts.push_back(vertex);
	}

	// Indices of new triangles
	vector<u32> tri_indices;

	while (true) {
		for (u32 i = 0; i < v_verts.size(); ++i) {

			// Previous, current, and next vertices
			vec3_f32 prev;
			vec3_f32 curr = v_verts[i].position;
			vec3_f32 next;

			// Previous vertex definition
			if (i == 0) {
				prev = v_verts[v_verts.size() - 1].position;
			}
			else {
				prev = v_verts[i - 1].position;
			}

			// Next vertex definition
			if (i == v_verts.size() - 1) {
				next = v_verts[0].position;
			}
			else {
				next = v_verts[i + 1].position;
			}


			// Create a triangle from previous, current, and next vertices
			if (v_verts.size() == 3) {
				for (u32 j = 0; j < v_verts.size(); ++j) {
					if (in_verts[j].position == prev)
						tri_indices.push_back(j);
					if (in_verts[j].position == curr)
						tri_indices.push_back(j);
					if (in_verts[j].position == next)
						tri_indices.push_back(j);
				}

				v_verts.clear();
				break;
			}

			if (v_verts.size() == 4) {
				// Create a triangle
				for (u32 j = 0; j < in_verts.size(); ++j) {
					if (in_verts[j].position == prev)
						tri_indices.push_back(j);
					if (in_verts[j].position == curr)
						tri_indices.push_back(j);
					if (in_verts[j].position == next)
						tri_indices.push_back(j);
				}

				vec3_f32 temp;
				for (u32 j = 0; j < v_verts.size(); ++j) {
					if (v_verts[j].position != prev &&
					    v_verts[j].position != curr &&
					    v_verts[j].position != next) {
						temp = v_verts[j].position;
						break;
					}
				}

				// Create a triangle
				for (u32 j = 0; j < in_verts.size(); ++j) {
					if (in_verts[j].position == prev)
						tri_indices.push_back(j);
					if (in_verts[j].position == next)
						tri_indices.push_back(j);
					if (in_verts[j].position == temp)
						tri_indices.push_back(j);
				}

				v_verts.clear();
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
			for (u32 j = 0; j < in_verts.size(); ++j) {
				if (PointInTriangle(prev, curr, next, in_verts[j].position)
				    && in_verts[j].position != prev
				    && in_verts[j].position != curr
				    && in_verts[j].position != next) {
					in_triangle = true;
					break;
				}
			}
			if (in_triangle) {
				continue;
			}

			// Create a triangle from previous, current, and next vertices
			for (u32 j = 0; j < in_verts.size(); ++j) {
				if (in_verts[j].position == prev)
					tri_indices.push_back(j);
				if (in_verts[j].position == curr)
					tri_indices.push_back(j);
				if (in_verts[j].position == next)
					tri_indices.push_back(j);
			}

			// Delete current vertex from the list
			for (u32 j = 0; j < v_verts.size(); ++j) {
				if (v_verts[j].position == curr) {
					v_verts.erase(v_verts.begin() + j);
					break;
				}
			}

			// Reset i
			i = -1;
		}


		// If no tris were created, then break
		if (indices.empty()) {
			break;
		}

		// If there are no more vertices, then break
		if (v_verts.empty()) {
			break;
		}
	}


	// Rebuild the face definition in terms of triangles
	vector<vec3_u32> temp_face = face_def;
	face_def.clear();
	for (const auto& index : tri_indices) {
		face_def.push_back(temp_face[index]);
	}
}

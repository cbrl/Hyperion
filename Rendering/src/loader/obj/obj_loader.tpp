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
	ThrowIfFailed(fs::exists(filename),
	              "Error loading file");

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
		throw std::exception("Could not open obj file");
	}


	//----------------------------------------------------------------------------------
	// Read file contents
	//----------------------------------------------------------------------------------

	wstring line;
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		if (line[0] == ObjTokens::comment || line == L"") {
			continue;
		}

		// Copy the first token from the line
		wstring token = line.substr(0, line.find_first_of(L" "));

		// Remove the token from the line
		line = line.substr(token.size());
		line = TrimWhiteSpace(line);

		// Create a stringstream to easily read data into variables
		wstringstream stream(line);


		// Vertex
		if (token.compare(ObjTokens::vertex) == 0) {
			f32_3 position;
			stream >> position.x >> position.y >> position.z;

			if (rh_coord) {
				position.z *= -1.0f;
			}

			vertex_positions.push_back(position);
		}

		// Normal
		else if (token.compare(ObjTokens::normal) == 0) {
			f32_3 normal;
			stream >> normal.x >> normal.y >> normal.z;

			if (rh_coord) {
				normal.z *= -1.0f;
			}

			vertex_normals.push_back(normal);
		}

		// Texture
		else if (token.compare(ObjTokens::texture) == 0) {
			f32_2 texCoord;
			stream >> texCoord.x >> texCoord.y;

			if (rh_coord) {
				texCoord.y = 1.0f - texCoord.y;
			}

			vertex_texCoords.push_back(texCoord);
		}

		// Group
		else if (token.compare(ObjTokens::group) == 0) {
			wstring name;
			stream >> name;

			groups.push_back(Group());
			groups.back().name        = wstr2str(name);
			groups.back().index_start = static_cast<u32>(indices.size());

			++group_count;
		}

		// Face
		else if (token.compare(ObjTokens::face) == 0) {
			readFace(line);
		}

		// Material Library
		if (token.compare(ObjTokens::mtl_library) == 0) {
			mat_lib = TrimWhiteSpace(line);
		}

		// Group Material
		else if (token.compare(ObjTokens::use_mtl) == 0) {
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
		throw std::exception("Could not open mtl file");
	}


	//----------------------------------------------------------------------------------
	// Read file contents
	//----------------------------------------------------------------------------------

	wstring line;
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		if (line[0] == ObjTokens::comment || line == L"") {
			continue;
		}

		// Copy the first token from the line
		wstring token = line.substr(0, line.find_first_of(L" "));

		// Remove the token from the line
		line = line.substr(token.size());
		line = TrimWhiteSpace(line);

		// Create a stringstream to easily read data into variables
		wstringstream stream(line);


		// New Material
		if (token.compare(ObjTokens::new_mtl) == 0) {
			ObjMaterial temp;
			temp.name = TrimWhiteSpace(line);
			materials.push_back(temp);

			++mtl_count;
		}

		// Diffuse Color
		else if (token.compare(ObjTokens::diffuse_color) == 0) {
			stream >> materials[mtl_count - 1].Kd.x;
			stream >> materials[mtl_count - 1].Kd.y;
			stream >> materials[mtl_count - 1].Kd.z;
			materials[mtl_count - 1].Kd.w = 1.0f;
		}

		// Ambient Color
		else if (token.compare(ObjTokens::ambient_color) == 0) {
			stream >> materials[mtl_count - 1].Ka.x;
			stream >> materials[mtl_count - 1].Ka.y;
			stream >> materials[mtl_count - 1].Ka.z;
			materials[mtl_count - 1].Ka.w = 1.0f;
		}

		// Specular Color
		else if (token.compare(ObjTokens::specular_color) == 0) {
			stream >> materials[mtl_count - 1].Ks.x;
			stream >> materials[mtl_count - 1].Ks.y;
			stream >> materials[mtl_count - 1].Ks.z;
		}

		// Emissive Color
		else if (token.compare(ObjTokens::emissive_color) == 0) {
			stream >> materials[mtl_count - 1].Ke.x;
			stream >> materials[mtl_count - 1].Ke.y;
			stream >> materials[mtl_count - 1].Ke.z;
			materials[mtl_count - 1].Ke.w = 1.0f;
		}

		// Specular Expononet
		else if (token.compare(ObjTokens::specular_exponent) == 0) {
			stream >> materials[mtl_count - 1].Ks.w;
		}

		// Optical Density
		else if (token.compare(ObjTokens::optical_density) == 0) {
			stream >> materials[mtl_count - 1].Ni;
		}

		// Dissolve (transparency)
		else if (token.compare(ObjTokens::transparency) == 0) {
			readTransparency(line, false);
		}

		// Dissolve (transparency inverse)
		else if (token.compare(ObjTokens::transparency_inv) == 0) {
			readTransparency(line, true);
		}

		// Illumination
		else if (token.compare(ObjTokens::illumination_model) == 0) {
			stream >> materials[mtl_count - 1].illum;
		}

		// Diffuse Map
		else if (token.compare(ObjTokens::diffuse_color_map) == 0) {
			materials[mtl_count - 1].map_Kd = TrimWhiteSpace(line);
		}

		// Alpha Map
		else if (token.compare(ObjTokens::alpha_texture_map) == 0) {
			materials[mtl_count - 1].map_d        = TrimWhiteSpace(line);
			materials[mtl_count - 1].transparency = true;
		}

		// Ambient Map
		else if (token.compare(ObjTokens::ambient_color_map) == 0) {
			materials[mtl_count - 1].map_Ka = TrimWhiteSpace(line);
		}

		// Specular Map
		else if (token.compare(ObjTokens::specular_color_map) == 0) {
			materials[mtl_count - 1].map_Ks = TrimWhiteSpace(line);
		}

		// Specular Highlight Map
		else if (token.compare(ObjTokens::spec_highlight_map) == 0) {
			materials[mtl_count - 1].map_Ns = TrimWhiteSpace(line);
		}

		// Bump Map
		else if (token.compare(ObjTokens::bump_map) == 0 || token.compare(ObjTokens::bump_map2) == 0) {
			materials[mtl_count - 1].map_bump = TrimWhiteSpace(line);
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

	float transparency;
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

	vector<VertexT> verts;
	VertexT vertex = {};

	bool has_normal = false;

	// Split the line into separate vertex definitions
	vector<wstring> vert_list = Split(line, L" ");

	for (size_t i = 0; i < vert_list.size(); ++i) {
		i32 type  = 0;

		// Split the vertex definition into separate parts
		vector<wstring> vert_parts = Split(vert_list[i], L"/");


		// Determine the vertex type
		switch (vert_parts.size()) {
			// Position
			case 1:
				type = 1;
				break;

			// Position/Texture
			case 2:
				type = 2;
				break;

			case 3:
				// Position/Normal
				if (vert_parts[1] == L"") type = 3;
				// Position/Texture/Normal
				else type = 4;
				break;

			default:
				break;
		}

		// Create the vertex definition and add it to the temporary vector
		switch (type) {
			// Position
			case 1: {
				vertex.position = vertex_positions[stoi(vert_parts[0]) - 1];
				//Subtract 1 since arrays start at index 0

				vertex.texCoord = f32_2(0.0f, 0.0f);

				has_normal = false;

				verts.push_back(vertex);
				break;
			}

			// Position/Texture
			case 2: {
				vertex.position = vertex_positions[stoi(vert_parts[0]) - 1];

				vertex.texCoord = vertex_texCoords[stoi(vert_parts[1]) - 1];

				has_normal = false;

				verts.push_back(vertex);
				break;
			}

			// Position/Normal
			case 3: {
				vertex.position = vertex_positions[stoi(vert_parts[0]) - 1];

				vertex.normal = vertex_normals[stoi(vert_parts[2]) - 1];

				vertex.texCoord = f32_2(0.0f, 0.0f);


				has_normal = true;

				verts.push_back(vertex);
				break;
			}

			// Position/Texture/Normal
			case 4: {
				vertex.position = vertex_positions[stoi(vert_parts[0]) - 1];

				vertex.normal = vertex_normals[stoi(vert_parts[2]) - 1];

				vertex.texCoord = vertex_texCoords[stoi(vert_parts[1]) - 1];

				has_normal = true;

				verts.push_back(vertex);
				break;
			}

			default:
				break;
		}
	}

	// Generate normals if the vertex type has a normal, but the file doesn't
	if (!has_normal && VertexT::hasNormal()) {
		const f32_3 p1 = verts[0].position - verts[1].position;
		const f32_3 p2 = verts[2].position - verts[1].position;

		const XMVECTOR a = XMLoad(&p1);
		const XMVECTOR b = XMLoad(&p2);

		for (size_t j = 0; j < verts.size(); ++j) {
			XMStore(&verts[j].normal, XMVector3Cross(a, b));
		}
	}

	// Add vertices to vector if they're not in it already
	for (size_t i = 0; i < verts.size(); ++i) {
		auto pos  = std::find(vertices.begin(), vertices.end(), verts[i]);
		if (pos == vertices.end()) {
			vertices.push_back(verts[i]);
		}
	}

	// If no group has been defined, then create one manually
	if (group_count == 0) {
		groups.push_back(Group());
		groups.back().index_start = static_cast<u32>(indices.size());
		++group_count;
	}

	// Add indices to index vector if the face was a triangle
	if (vert_list.size() == 3) {
		// Get the index of each vertex and add it to the index vector
		for (size_t i = 0; i < vert_list.size(); ++i) {

			auto pos = std::find(vertices.begin(), vertices.end(), verts[i]);
			if (pos != vertices.end()) {
				auto index = std::distance(vertices.begin(), pos);
				indices.push_back(static_cast<u32>(index));
			}
		}
	}

	// Triangulate the face if there were more than 3 points
	else if (vert_list.size() > 3) {
		vector<u32> v_indices;

		// Triangulate the vertices
		triangulate(verts, v_indices);

		// Add the new indices to the vector
		for (size_t i = 0; i < v_indices.size(); ++i) {
			// Find the index of each vertex and add it to the index vector
			auto pos = std::find(vertices.begin(), vertices.end(), verts[v_indices[i]]);

			if (pos != vertices.end()) {
				auto index = std::distance(vertices.begin(), pos);
				indices.push_back(static_cast<u32>(index));
			}
		}
	}
}


// Converts the input face into triangles. Returns a list of indices that correspond to the input vertex list.
template<typename VertexT>
void OBJLoader<VertexT>::triangulate(vector<VertexT>& in_verts, vector<u32>& out_indices) {
	vector<VertexT> v_verts = in_verts;

	while (true) {
		for (u32 i = 0; i < v_verts.size(); ++i) {
			f32_3 prev;
			if (i == 0) {
				prev = v_verts[v_verts.size() - 1].position;
			}
			else {
				prev = v_verts[i - 1].position;
			}

			f32_3 curr = v_verts[i].position;

			f32_3 next;
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
						out_indices.push_back(j);
					if (in_verts[j].position == curr)
						out_indices.push_back(j);
					if (in_verts[j].position == next)
						out_indices.push_back(j);
				}

				v_verts.clear();
				break;
			}

			if (v_verts.size() == 4) {
				// Create a triangle
				for (u32 j = 0; j < in_verts.size(); ++j) {
					if (in_verts[j].position == prev)
						out_indices.push_back(j);
					if (in_verts[j].position == curr)
						out_indices.push_back(j);
					if (in_verts[j].position == next)
						out_indices.push_back(j);
				}

				f32_3 temp;
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
						out_indices.push_back(j);
					if (in_verts[j].position == next)
						out_indices.push_back(j);
					if (in_verts[j].position == temp)
						out_indices.push_back(j);
				}

				v_verts.clear();
				break;
			}


			// Ensure that the vertex isn't an interior vertex
			const f32_3 v1     = prev - curr;
			const f32_3 v2     = next - curr;
			const XMVECTOR vec1 = XMLoad(&v1);
			const XMVECTOR vec2 = XMLoad(&v2);

			float angle = XMVectorGetX(XMVector3AngleBetweenVectors(vec1, vec2));
			angle       = XMConvertToDegrees(angle);

			if (angle <= 0 || angle >= 180) {
				continue;
			}


			// Ensure that no vertices are inside the triangle
			bool in_triangle = false;
			for (u32 j       = 0; j < in_verts.size(); ++j) {
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
					out_indices.push_back(j);
				if (in_verts[j].position == curr)
					out_indices.push_back(j);
				if (in_verts[j].position == next)
					out_indices.push_back(j);
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
		if (indices.size() == 0) {
			break;
		}

		// If there are no more vertices, then break
		if (v_verts.size() == 0) {
			break;
		}
	}
}

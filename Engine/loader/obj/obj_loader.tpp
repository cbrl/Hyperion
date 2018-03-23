#include <algorithm>
#include "loader\obj\obj_tokens.h"
#include "util\io\io.h"
#include "util\math\math.h"


template<typename VertexT>
void OBJLoader<VertexT>::Reset() {
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
ModelOutput<VertexT> OBJLoader<VertexT>::Load(ID3D11Device* device,
											  ResourceMgr& resource_mgr,
											  const wstring& folder,
											  const wstring& filename,
											  bool right_hand_coords) {

	// Make sure the file exists first
	ThrowIfFailed(fs::exists(folder + filename),
	              "Error loading file");

	// Set the coordinate system
	rh_coord = right_hand_coords;


	// Load the model
	LoadModel(folder, filename);

	// Load the materials
	LoadMaterials(folder);

	
	// Create the materials
	vector<Material> mtlVector;
	for (u32 i = 0; i < mtl_count; ++i) {
		Material mtl;

		mtl.name = materials[i].name;
		
		if (!materials[i].map_Kd.empty())
			mtl.map_Kd = resource_mgr.Create<Texture>(folder + materials[i].map_Kd);
		else
			mtl.map_Kd = resource_mgr.Create<Texture>(float4(1.0f, 1.0f, 1.0f, 1.0f));

		if (!materials[i].map_Ka.empty())
			mtl.map_Ka   = resource_mgr.Create<Texture>(folder + materials[i].map_Ka);
		if (!materials[i].map_Ks.empty())
			mtl.map_Ks   = resource_mgr.Create<Texture>(folder + materials[i].map_Ks);
		if (!materials[i].map_Ns.empty())
			mtl.map_Ns   = resource_mgr.Create<Texture>(folder + materials[i].map_Ns);
		if (!materials[i].map_d.empty())
			mtl.map_d    = resource_mgr.Create<Texture>(folder + materials[i].map_d);
		if (!materials[i].map_bump.empty())
			mtl.map_bump = resource_mgr.Create<Texture>(folder + materials[i].map_bump);

		mtl.Ka = materials[i].Ka;
		mtl.Kd = materials[i].Kd;
		mtl.Ks = materials[i].Ks;
		mtl.Ke = materials[i].Ke;
		mtl.d  = materials[i].d;
		mtl.Ni = materials[i].Ni;
		mtl.illum = materials[i].illum;
		mtl.transparent = materials[i].transparency;

		mtlVector.push_back(mtl);
	}

	// Create the model
	wstring name = filename.substr(0, filename.find(L'.'));
	ModelOutput<VertexT> out(name, vertices, indices, mtlVector, groups);


	// Reset the obj loader
	Reset();


	return out;
}


template<typename VertexT>
void OBJLoader<VertexT>::LoadModel(wstring folder, wstring filename) {
	
	// Open the file
	wifstream file(folder + filename);
	if (file.fail()) {
		throw std::exception("Could not open obj file");
	}


	//----------------------------------------------------------------------------------
	// Read file contents
	//----------------------------------------------------------------------------------

	wstring line;
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		if (line[0] == OBJTokens::comment || line == L"") {
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
		if (token.compare(OBJTokens::vertex) == 0) {
			float3 position;
			stream >> position.x >> position.y >> position.z;

			if (rh_coord) {
				position.z *= -1.0f;
			}

			vertex_positions.push_back(position);
		}

		// Normal
		else if (token.compare(OBJTokens::normal) == 0) {
			float3 normal;
			stream >> normal.x >> normal.y >> normal.z;

			if (rh_coord) {
				normal.z *= -1.0f;
			}

			vertex_normals.push_back(normal);
		}

		// Texture
		else if (token.compare(OBJTokens::texture) == 0) {
			float2 texCoord;
			stream >> texCoord.x >> texCoord.y;

			if (rh_coord) {
				texCoord.y = 1.0f - texCoord.y;
			}

			vertex_texCoords.push_back(texCoord);
		}

		// Group
		else if (token.compare(OBJTokens::group) == 0) {
			groups.push_back(Group());
			stream >> groups.back().name;
			groups.back().index_start = static_cast<u32>(indices.size());

			++group_count;
		}

		// Face
		else if (token.compare(OBJTokens::face) == 0) {
			ReadFace(line);
		}

		// Material Library
		if (token.compare(OBJTokens::mtl_library) == 0) {
			mat_lib = TrimWhiteSpace(line);
		}

		// Group Material
		else if (token.compare(OBJTokens::use_mtl) == 0) {
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
void OBJLoader<VertexT>::LoadMaterials(wstring folder) {
	
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
		if (line[0] == OBJTokens::comment || line == L"") {
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
		if (token.compare(OBJTokens::new_mtl) == 0) {
			OBJMaterial temp;
			temp.name = TrimWhiteSpace(line);
			materials.push_back(temp);

			++mtl_count;
		}

		// Diffuse Color
		else if (token.compare(OBJTokens::diffuse_color) == 0) {
			stream >> materials[mtl_count - 1].Kd.x;
			stream >> materials[mtl_count - 1].Kd.y;
			stream >> materials[mtl_count - 1].Kd.z;
			materials[mtl_count - 1].Kd.w = 1.0f;
		}

		// Ambient Color
		else if (token.compare(OBJTokens::ambient_color) == 0) {
			stream >> materials[mtl_count - 1].Ka.x;
			stream >> materials[mtl_count - 1].Ka.y;
			stream >> materials[mtl_count - 1].Ka.z;
			materials[mtl_count - 1].Ka.w = 1.0f;
		}

		// Specular Color
		else if (token.compare(OBJTokens::specular_color) == 0) {
			stream >> materials[mtl_count - 1].Ks.x;
			stream >> materials[mtl_count - 1].Ks.y;
			stream >> materials[mtl_count - 1].Ks.z;
		}

		// Emissive Color
		else if (token.compare(OBJTokens::emissive_color) == 0) {
			stream >> materials[mtl_count - 1].Ke.x;
			stream >> materials[mtl_count - 1].Ke.y;
			stream >> materials[mtl_count - 1].Ke.z;
			materials[mtl_count - 1].Ke.w = 1.0f;
		}

		// Specular Expononet
		else if (token.compare(OBJTokens::specular_exponent) == 0) {
			stream >> materials[mtl_count - 1].Ks.w;
		}

		// Optical Density
		else if (token.compare(OBJTokens::optical_density) == 0) {
			stream >> materials[mtl_count - 1].Ni;
		}

		// Dissolve (transparency)
		else if (token.compare(OBJTokens::transparency) == 0) {
			ReadTransparency(line, false);
		}

		// Dissolve (transparency inverse)
		else if (token.compare(OBJTokens::transparency_inv) == 0) {
			ReadTransparency(line, true);
		}

		// Illumination
		else if (token.compare(OBJTokens::illumination_model) == 0) {
			stream >> materials[mtl_count - 1].illum;
		}

		// Diffuse Map
		else if (token.compare(OBJTokens::diffuse_color_map) == 0) {
			materials[mtl_count - 1].map_Kd = TrimWhiteSpace(line);
		}

		// Alpha Map
		else if (token.compare(OBJTokens::alpha_texture_map) == 0) {
			materials[mtl_count - 1].map_d = TrimWhiteSpace(line);
			materials[mtl_count - 1].transparency = true;
		}

		// Ambient Map
		else if (token.compare(OBJTokens::ambient_color_map) == 0) {
			materials[mtl_count - 1].map_Ka = TrimWhiteSpace(line);
		}

		// Specular Map
		else if (token.compare(OBJTokens::specular_color_map) == 0) {
			materials[mtl_count - 1].map_Ks = TrimWhiteSpace(line);
		}

		// Specular Highlight Map
		else if (token.compare(OBJTokens::spec_highlight_map) == 0) {
			materials[mtl_count - 1].map_Ns = TrimWhiteSpace(line);
		}

		// Bump Map
		else if (token.compare(OBJTokens::bump_map) == 0 || token.compare(OBJTokens::bump_map2) == 0) {
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
void OBJLoader<VertexT>::ReadTransparency(wstring& line, bool inverse) {
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
void OBJLoader<VertexT>::ReadFace(wstring& line) {
	vector<wstring> vert_list, vert_parts;

	vector<VertexPositionNormalTexture> verts;
	VertexPositionNormalTexture vertex;

	bool hasNormal = false;

	// Split the line into separate vertex definitions
	Split(line, vert_list, L" ");

	for (size_t i = 0; i < vert_list.size(); ++i) {
		i32 type = 0;

		// Split the vertex definition into separate parts
		Split(vert_list[i], vert_parts, L"/");


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
		}

		// Create the vertex definition and add it to the temporary vector
		switch (type) {
			// Position
			case 1:
			{
				vertex.position = GetElement(vertex_positions, stoi(vert_parts[0])-1);  //Subtract 1 since arrays start at index 0

				if (has_texCoord_v<VertexT>)
					vertex.texCoord = float2(0.0f, 0.0f);

				hasNormal = false;

				verts.push_back(vertex);
				break;
			}

			// Position/Texture
			case 2:
			{
				vertex.position = GetElement(vertex_positions, stoi(vert_parts[0])-1);

				if (has_texCoord_v<VertexT>)
					vertex.texCoord = GetElement(vertex_texCoords, stoi(vert_parts[1])-1);

				hasNormal = false;

				verts.push_back(vertex);
				break;
			}

			// Position/Normal
			case 3:
			{
				vertex.position = GetElement(vertex_positions, stoi(vert_parts[0])-1);

				if (has_normal_v<VertexT>) {
					vertex.normal = GetElement(vertex_normals, stoi(vert_parts[2]) - 1);
				}

				if (has_texCoord_v<VertexT>)
					vertex.texCoord = float2(0.0f, 0.0f);


				hasNormal = true;

				verts.push_back(vertex);
				break;
			}

			// Position/Texture/Normal
			case 4:
			{
				vertex.position = GetElement(vertex_positions, stoi(vert_parts[0])-1);

				if (has_normal_v<VertexT>)
					vertex.normal = GetElement(vertex_normals, stoi(vert_parts[2])-1);

				if (has_texCoord_v<VertexT>)
					vertex.texCoord = GetElement(vertex_texCoords, stoi(vert_parts[1])-1);

				hasNormal = true;

				verts.push_back(vertex);
				break;
			}

			default:
				break;
		}

		// Generate normals if the vertex type has a normal, but the file doesn't
		if (!hasNormal && has_normal_v<VertexT>) {
			XMVECTOR a = XMLoadFloat3(&(verts[0].position - verts[1].position));
			XMVECTOR b = XMLoadFloat3(&(verts[2].position - verts[1].position));

			float3 normal;
			XMStoreFloat3(&normal, XMVector3Cross(a, b));

			for (size_t i = 0; i < verts.size(); ++i) {
				verts[i].normal = normal;
			}
		}
	}

	// Add vertices to vector if they're not in it already
	for (size_t i = 0; i < verts.size(); ++i) {
		auto pos = std::find(vertices.begin(), vertices.end(), verts[i]);
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
		vector<u32> vIndices;

		// Triangulate the vertices
		Triangulate(verts, vIndices);

		// Add the new indices to the vector
		for (size_t i = 0; i < vIndices.size(); ++i) {
			// Find the index of each vertex and add it to the index vector
			auto pos = std::find(vertices.begin(), vertices.end(), verts[vIndices[i]]);

			if (pos != vertices.end()) {
				auto index = std::distance(vertices.begin(), pos);
				indices.push_back(static_cast<u32>(index));
			}
		}
	}
}


// Converts the input face into triangles. Returns a list of indices that correspond to the input vertex list.
template<typename VertexT>
void OBJLoader<VertexT>::Triangulate(vector<VertexPositionNormalTexture>& inVerts, vector<u32>& outIndices) {
	vector<VertexPositionNormalTexture> vVerts = inVerts;

	while (true) {
		for (u32 i = 0; i < vVerts.size(); ++i) {
			float3 prev;
			if (i == 0) {
				prev = vVerts[vVerts.size() - 1].position;
			}
			else {
				prev = vVerts[i - 1].position;
			}

			float3 curr = vVerts[i].position;

			float3 next;
			if (i == vVerts.size() - 1) {
				next = vVerts[0].position;
			}
			else {
				next = vVerts[i + 1].position;
			}


			// Create a triangle from previous, current, and next vertices
			if (vVerts.size() == 3) {
				for (u32 j = 0; j < vVerts.size(); ++j) {
					if (inVerts[j].position == prev)
						outIndices.push_back(j);
					if (inVerts[j].position == curr)
						outIndices.push_back(j);
					if (inVerts[j].position == next)
						outIndices.push_back(j);
				}

				vVerts.clear();
				break;
			}

			if (vVerts.size() == 4) {
				// Create a triangle
				for (u32 j = 0; j < inVerts.size(); ++j) {
					if (inVerts[j].position == prev)
						outIndices.push_back(j);
					if (inVerts[j].position == curr)
						outIndices.push_back(j);
					if (inVerts[j].position == next)
						outIndices.push_back(j);
				}

				float3 temp;
				for (u32 j = 0; j < vVerts.size(); ++j) {
					if (vVerts[j].position != prev &&
						vVerts[j].position != curr &&
						vVerts[j].position != next)
					{
						temp = vVerts[j].position;
						break;
					}
				}

				// Create a triangle
				for (u32 j = 0; j < inVerts.size(); ++j) {
					if (inVerts[j].position == prev)
						outIndices.push_back(j);
					if (inVerts[j].position == next)
						outIndices.push_back(j);
					if (inVerts[j].position == temp)
						outIndices.push_back(j);
				}

				vVerts.clear();
				break;
			}


			// Ensure that the vertex isn't an interior vertex
			XMVECTOR vec1 = XMLoadFloat3(&(prev - curr));
			XMVECTOR vec2 = XMLoadFloat3(&(next - curr));

			float angle = XMVectorGetX(XMVector3AngleBetweenVectors(vec1, vec2));
			angle = XMConvertToDegrees(angle);

			if (angle <= 0 || angle >= 180) {
				continue;
			}


			// Ensure that no vertices are inside the triangle
			bool inTriangle = false;
			for (u32 j = 0; j < inVerts.size(); ++j) {
				if (PointInTriangle(inVerts[j].position, prev, curr, next)
					&& inVerts[j].position != prev
					&& inVerts[j].position != curr
					&& inVerts[j].position != next)
				{
					inTriangle = true;
					break;
				}
			}
			if (inTriangle) {
				continue;
			}

			// Create a triangle from previous, current, and next vertices
			for (u32 j = 0; j < inVerts.size(); ++j) {
				if (inVerts[j].position == prev)
					outIndices.push_back(j);
				if (inVerts[j].position == curr)
					outIndices.push_back(j);
				if (inVerts[j].position == next)
					outIndices.push_back(j);
			}

			// Delete current vertex from the list
			for (u32 j = 0; j < vVerts.size(); ++j) {
				if (vVerts[j].position == curr) {
					vVerts.erase(vVerts.begin() + j);
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
		if (vVerts.size() == 0) {
			break;
		}
	}
}

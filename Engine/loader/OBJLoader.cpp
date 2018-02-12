#include "stdafx.h"
#include "OBJLoader.h"
#include "loader\OBJTokens.h"
#include "direct3d\Direct3D.h"
#include "texture\TextureMgr.h"


OBJLoader::OBJLoader() :
	RHcoord(false),
	hasNormals(false),
	hasTexture(false),
	groupCount(0),
	mtlCount(0)
{
}


OBJLoader::~OBJLoader() {
}


Model OBJLoader::Load(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring folder, wstring filename, bool RHcoordinates) {
	RHcoord = RHcoordinates;

	LoadModel(folder, filename);
	LoadMaterials(folder);


	//----------------------------------------------------------------------------------
	// Create Model
	//----------------------------------------------------------------------------------

	// Create the vertices and store in a vector
	//vector<VertexPositionNormalTexture> vertices;
	//VertexPositionNormalTexture vertex;
	//for (int i = 0; i < vTotal; ++i) {
	//	vertex.position = vPositions[vPositionIndices[i]];
	//	vertex.normal = vNormals[vNormalIndices[i]];
	//	vertex.textureCoordinate = vTexCoords[vTexCoordIndices[i]];

	//	vertices.push_back(vertex);
	//}

	
	// Create the materials
	vector<Material> mtlVector;
	for (int i = 0; i < mtlCount; ++i) {
		Material mtl;
		mtl.name = materials[i].name;
		
		if (!materials[i].map_Ka.empty())
			mtl.map_Ka   = TextureMgr::Get()->Texture(device, deviceContext, folder + materials[i].map_Ka);
		if (!materials[i].map_Kd.empty())
			mtl.map_Kd   = TextureMgr::Get()->Texture(device, deviceContext, folder + materials[i].map_Kd);
		if (!materials[i].map_Ks.empty())
			mtl.map_Ks   = TextureMgr::Get()->Texture(device, deviceContext, folder + materials[i].map_Ks);
		if (!materials[i].map_Ns.empty())
			mtl.map_Ns   = TextureMgr::Get()->Texture(device, deviceContext, folder + materials[i].map_Ns);
		if (!materials[i].map_d.empty())
			mtl.map_d    = TextureMgr::Get()->Texture(device, deviceContext, folder + materials[i].map_d);
		if (!materials[i].map_bump.empty())
			mtl.map_bump = TextureMgr::Get()->Texture(device, deviceContext, folder + materials[i].map_bump);

		mtl.Ka = materials[i].Ka;
		mtl.Kd = materials[i].Kd;
		mtl.Ks = materials[i].Ks;
		mtl.Ns = materials[i].Ns;
		mtl.d  = materials[i].d;
		mtl.Ni = materials[i].Ni;
		mtl.illum = materials[i].illum;
		mtl.transparent = materials[i].transparency;

		mtlVector.push_back(mtl);
	}

	Mesh mesh(device, vertices, indices, mtlVector, groupCount, groupVertexIndices, groupMaterialIndices);
	Model model(mesh);


	// Constuct model
	// Maybe make this a mesh, and have another class that constructs the model
	//Model<VertexPositionNormalTexture> model;

	//DX::ThrowIfFailed(model.Init(Direct3D::Get()->GetDevice().Get(), vertices, indices, texture));


	// RESET


	return model;
}


void OBJLoader::LoadModel(wstring folder, wstring filename) {
	
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
			XMFLOAT3 position;
			stream >> position.x >> position.y >> position.z;

			if (RHcoord) {
				position.z *= -1.0f;
			}

			vPositions.push_back(position);
		}

		// Normal
		else if (token.compare(OBJTokens::normal) == 0) {
			XMFLOAT3 normal;
			stream >> normal.x >> normal.y >> normal.z;

			if (RHcoord) {
				normal.z *= -1.0f;
			}

			vNormals.push_back(normal);
			hasNormals = true;
		}

		// Texture
		else if (token.compare(OBJTokens::texture) == 0) {
			XMFLOAT2 texCoord;
			stream >> texCoord.x >> texCoord.y;

			if (RHcoord) {
				texCoord.y = 1.0f - texCoord.y;
			}

			vTexCoords.push_back(texCoord);
			hasTexture = true;
		}

		// Group
		else if (token.compare(OBJTokens::group) == 0) {
			groupVertexIndices.push_back(indices.size());
			++groupCount;
		}

		// Face
		else if (token.compare(OBJTokens::face) == 0) {
			ReadFace(line);
		}

		// Material Library
		if (token.compare(OBJTokens::mtl_library) == 0) {
			meshMatLib = TrimWhiteSpace(line);
		}

		// Group Material
		else if (token.compare(OBJTokens::group_mtl) == 0) {
			meshMaterials[groupCount - 1] = TrimWhiteSpace(line);
		}
	}

	// Close the OBJ file
	file.close();


	// There won't be another index start after the last group, so set it here
	groupVertexIndices.push_back(indices.size());

	// Sometimes a group is defined at the top of the file, then an extra one
	// before the first vertex. If that happened, then remove the extra.
	if (groupVertexIndices[1] == 0) {
		groupVertexIndices.erase(groupVertexIndices.begin() + 1);
		--groupCount;
	}

	// Make sure there is a default texCoord and normal
	if (!hasNormals) {
		vNormals.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	if (!hasTexture) {
		vTexCoords.push_back(XMFLOAT2(0.0f, 0.0f));
	}
}


void OBJLoader::LoadMaterials(wstring folder) {
	
	// Open the material file
	wifstream file(folder + meshMatLib);
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

			++mtlCount;
		}

		// Diffuse Color
		else if (token.compare(OBJTokens::diffuse_color) == 0) {
			stream >> materials[mtlCount - 1].Kd.x;
			stream >> materials[mtlCount - 1].Kd.y;
			stream >> materials[mtlCount - 1].Kd.z;
		}

		// Ambient Color
		else if (token.compare(OBJTokens::ambient_color) == 0) {
			stream >> materials[mtlCount - 1].Ka.x;
			stream >> materials[mtlCount - 1].Ka.y;
			stream >> materials[mtlCount - 1].Ka.z;
		}

		// Dissolve (transparency)
		else if (token.compare(OBJTokens::transparency) == 0) {
			ReadTransparency(line, false);
		}

		// Dissolve (transparency)
		else if (token.compare(OBJTokens::transparency_inv) == 0) {
			ReadTransparency(line, true);
		}

		// Specular Color
		else if (token.compare(OBJTokens::specular_color) == 0) {
			stream >> materials[mtlCount - 1].Ks.x;
			stream >> materials[mtlCount - 1].Ks.y;
			stream >> materials[mtlCount - 1].Ks.z;
		}

		// Specular Expononet
		else if (token.compare(OBJTokens::specular_exponent) == 0) {
			stream >> materials[mtlCount - 1].Ns;
		}

		else if (token.compare(OBJTokens::emissive_color) == 0) {
			// READ EMISSIVE COLOR
		}

		// Optical Density
		else if (token.compare(OBJTokens::optical_density) == 0) {
			stream >> materials[mtlCount - 1].Ni;
		}

		// Illumination
		else if (token.compare(OBJTokens::illumination_model) == 0) {
			stream >> materials[mtlCount - 1].illum;
		}

		// Diffuse Map
		else if (token.compare(OBJTokens::diffuse_color_map) == 0) {
			materials[mtlCount - 1].map_Kd = TrimWhiteSpace(line);
		}

		// Alpha Map
		else if (token.compare(OBJTokens::alpha_texture_map) == 0) {
			materials[mtlCount - 1].map_d = TrimWhiteSpace(line);
			materials[mtlCount - 1].transparency = true;
		}

		// Ambient Map
		else if (token.compare(OBJTokens::ambient_color_map) == 0) {
			materials[mtlCount - 1].map_Ka = TrimWhiteSpace(line);
		}

		// Specular Map
		else if (token.compare(OBJTokens::specular_color_map) == 0) {
			materials[mtlCount - 1].map_Ks = TrimWhiteSpace(line);
		}

		// Specular Highlight Map
		else if (token.compare(OBJTokens::spec_highlight_map) == 0) {
			materials[mtlCount - 1].map_Ns = TrimWhiteSpace(line);
		}

		// Bump Map
		else if (token.compare(OBJTokens::bump_map) == 0 || token.compare(OBJTokens::bump_map2) == 0) {
			materials[mtlCount - 1].map_bump = TrimWhiteSpace(line);
		}
	}

	// Close the material file
	file.close();


	// Set the group material to the index value of its
	// material in the material vector.
	for (int i = 0; i < groupCount; ++i) {
		bool hasMat = false;

		// If the group doesn't have a material set, then
		// use the first material.
		if (meshMaterials[i].empty()) {
			meshMaterials[i] = materials[0].name;
		}

		for (int j = 0; j < materials.size(); ++j) {
			if (meshMaterials[i] == materials[j].name) {
				groupMaterialIndices.push_back(j);
				hasMat = true;
			}
		}

		if (!hasMat) {
			// Use first material in vector
			groupMaterialIndices.push_back(0);
		}
	}
}


void OBJLoader::ReadTransparency(wstring& line, bool inverse) {
	wstringstream stream(line);

	float transparency;
	stream >> transparency;

	if (inverse) {
		transparency = 1.0f - transparency;
	}

	materials[mtlCount - 1].d = transparency;

	if (transparency > 0.0f) {
		materials[mtlCount - 1].transparency = true;
	}
}


void OBJLoader::ReadFace(wstring& line) {
	vector<wstring> vList, vParts;

	vector<VertexPositionNormalTexture> vVerts;
	VertexPositionNormalTexture vertex;

	bool hasNormal = false;

	// Split the line into separate vertex definitions
	Split(line, vList, L" ");

	for (size_t i = 0; i < vList.size(); ++i) {
		int type = 0;

		// Split the vertex definition into separate parts
		Split(vList[i], vParts, L"/");

		switch (vParts.size()) {
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
				if (vParts[1] == L"") type = 3;
				// Position/Texture/Normal
				else type = 4;
				break;
		}

		switch (type) {
			// Position
			case 1:
			{
				vertex.position = GetElement(vPositions, stoi(vParts[0])-1);
				vertex.textureCoordinate = XMFLOAT2(0.0f, 0.0f);
				hasNormal = false;

				vVerts.push_back(vertex);
				break;
			}

			// Position/Texture
			case 2:
			{
				vertex.position = GetElement(vPositions, stoi(vParts[0])-1);
				vertex.textureCoordinate = GetElement(vTexCoords, stoi(vParts[1])-1);
				hasNormal = false;

				vVerts.push_back(vertex);
				break;
			}

			// Position/Normal
			case 3:
			{
				vertex.position = GetElement(vPositions, stoi(vParts[0])-1);
				vertex.normal = GetElement(vNormals, stoi(vParts[2])-1);
				vertex.textureCoordinate = XMFLOAT2(0.0f, 0.0f);
				hasNormal = true;

				vVerts.push_back(vertex);
				break;
			}

			// Position/Texture/Normal
			case 4:
			{
				vertex.position = GetElement(vPositions, stoi(vParts[0])-1);
				vertex.normal = GetElement(vNormals, stoi(vParts[2])-1);
				vertex.textureCoordinate = GetElement(vTexCoords, stoi(vParts[1])-1);
				hasNormal = true;

				vVerts.push_back(vertex);
				break;
			}

			default:
				break;
		}

		if (!hasNormal) {
			// CALCULATE NORMALS
		}
	}

	// Add vertices to vector
	for (size_t i = 0; i < vVerts.size(); ++i) {
		vertices.push_back(vVerts[i]);
	}



	// If no group has been defined, then create one manually
	if (groupCount == 0) {
		groupVertexIndices.push_back(indices.size());
		++groupCount;
	}

	// Add indices to vector if the face was a triangle
	if (vVerts.size() == 3) {
		indices.push_back(vertices.size());
		indices.push_back(vertices.size() + 1);
		indices.push_back(vertices.size() + 2);
	}

	// Triangulate the face if there were more than 3 points
	else if (vVerts.size() > 3) {
		vector<UINT> vIndices;

		Triangulate(vVerts, vIndices);
	}

}


void OBJLoader::Triangulate(vector<VertexPositionNormalTexture>& inVerts, vector<UINT>& outIndices) {
	vector<VertexPositionNormalTexture> vVerts = inVerts;

	for (size_t i = 0; i < vVerts.size(); ++i) {



		while (true) {
			XMFLOAT3 prev;
			if (i == 0) {
				prev = vVerts[vVerts.size() - 1].position;
			}
			else {
				prev = vVerts[i - 1].position;
			}

			XMFLOAT3 curr = vVerts[i].position;

			XMFLOAT3 next;
			if (i == vVerts.size() - 1) {
				next = vVerts[0].position;
			}
			else {
				next = vVerts[i + 1].position;
			}


			// Create a triangle from previous, current, and next vertices
			if (vVerts.size() == 3) {
				for (size_t j = 0; j < vVerts.size(); ++j) {
					if (XMFloat3Equal(inVerts[j].position, prev))
						indices.push_back(j);
					if (XMFloat3Equal(inVerts[j].position, curr))
						indices.push_back(j);
					if (XMFloat3Equal(inVerts[j].position, next))
						indices.push_back(j);
				}

				vVerts.clear();
				break;
			}

			if (vVerts.size() == 4) {
				// Create a triangle
				for (size_t j = 0; j < inVerts.size(); ++j) {
					if (XMFloat3Equal(inVerts[j].position, prev))
						indices.push_back(j);
					if (XMFloat3Equal(inVerts[j].position, curr))
						indices.push_back(j);
					if (XMFloat3Equal(inVerts[j].position, next))
						indices.push_back(j);
				}

				XMFLOAT3 temp;
				for (size_t j = 0; j < vVerts.size(); ++j) {
					if (!XMFloat3Equal(vVerts[j].position, prev) &&
						!XMFloat3Equal(vVerts[j].position, curr) &&
						!XMFloat3Equal(vVerts[j].position, next)) {
						temp = vVerts[j].position;
						break;
					}
				}

				// Create a triangle
				for (size_t j = 0; j < inVerts.size(); ++j) {
					if (XMFloat3Equal(inVerts[j].position, prev))
						indices.push_back(j);
					if (XMFloat3Equal(inVerts[j].position, next))
						indices.push_back(j);
					if (XMFloat3Equal(inVerts[j].position, temp))
						indices.push_back(j);
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
			for (size_t j = 0; j < inVerts.size(); ++j) {
				if (PointInTriangle(inVerts[j].position, prev, curr, next)
					&& !XMFloat3Equal(inVerts[j].position, prev)
					&& !XMFloat3Equal(inVerts[j].position, curr)
					&& !XMFloat3Equal(inVerts[j].position, next)) {
					inTriangle = true;
					break;
				}
			}
			if (inTriangle) {
				continue;
			}

			// Create a triangle from previous, current, and next vertices
			for (size_t j = 0; j < inVerts.size(); ++j) {
				if (XMFloat3Equal(inVerts[j].position, prev))
					indices.push_back(j);
				if (XMFloat3Equal(inVerts[j].position, curr))
					indices.push_back(j);
				if (XMFloat3Equal(inVerts[j].position, next))
					indices.push_back(j);
			}

			// Delete current vertex from the list
			for (size_t j = 0; j < vVerts.size(); ++j) {
				if (XMFloat3Equal(vVerts[j].position, curr)) {
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

#include "stdafx.h"
#include "OBJLoader.h"
#include "loader\OBJTokens.h"
#include "direct3d\Direct3D.h"
#include "texture\TextureMgr.h"


OBJLoader::OBJLoader() :
	RHcoord(false),
	hasNormals(false),
	hasTexture(false),
	kdSet(false),
	vIndex(0),
	vTotal(0),
	groupCount(0),
	triangleCount(0),
	meshTriangles(0),
	materialCount(0)
{
}


OBJLoader::~OBJLoader() {
}


wstring OBJLoader::TrimWhiteSpace(wstring& in) {
	size_t textStart = in.find_first_not_of(L" \t");
	size_t textEnd   = in.find_last_not_of(L" \t");

	if (textStart != wstring::npos && textEnd != wstring::npos) {
		return in.substr(textStart, textEnd - textStart + 1);
	}
	else if (textStart != wstring::npos) {
		return in.substr(textStart);
	}

	return L"";
}


Model OBJLoader::Load(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring folder, wstring filename, bool RHcoordinates) {
	RHcoord = RHcoordinates;

	wstring line;

	//----------------------------------------------------------------------------------
	// OBJ File
	//----------------------------------------------------------------------------------

	wifstream file(folder + filename);
	if (file.fail()) {
		throw std::exception("Could not open obj file");
	}

	// Read file contents
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		ReadLine(line);
	}

	// Close the OBJ file
	file.close();
	file.clear();

	// There won't be another index start after the last group, so set it here
	groupIndexStart.push_back(vIndex);

	// Sometimes a group is defined at the top of the file, then an extra one
	// before the first vertex. If that happened, then remove the extra.
	if (groupIndexStart[1] == 0) {
		groupIndexStart.erase(groupIndexStart.begin() + 1);
		--groupCount;
	}

	// Make sure there is a default texCoord and normal
	if (!hasNormals) {
		vNormals.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	if (!hasTexture) {
		vTexCoords.push_back(XMFLOAT2(0.0f, 0.0f));
	}



	//----------------------------------------------------------------------------------
	// Material File
	//----------------------------------------------------------------------------------

	file.open(folder + meshMatLib);
	if (file.fail()) {
		throw std::exception("Could not open mtl file");
	}
	
	while (std::getline(file, line)) {
		line = TrimWhiteSpace(line);
		ReadLine(line);
	}

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
				groupMaterials.push_back(j);
				hasMat = true;
			}
		}

		if (!hasMat) {
			// Use first material in vector
			groupMaterials.push_back(0);
		}
	}



	//----------------------------------------------------------------------------------
	// Create Model
	//----------------------------------------------------------------------------------

	// Create the vertices and store in a vector
	vector<VertexPositionNormalTexture> vertices;
	VertexPositionNormalTexture vertex;
	for (int i = 0; i < vTotal; ++i) {
		vertex.position = vPositions[vPositionIndices[i]];
		vertex.normal = vNormals[vNormalIndices[i]];
		vertex.textureCoordinate = vTexCoords[vTexCoordIndices[i]];

		vertices.push_back(vertex);
	}


	// Create the materials
	vector<Material> mtlVector;
	for (int i = 0; i < materialCount; ++i) {
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

	Mesh mesh(device, vertices, indices, groupCount, groupIndexStart, mtlVector, groupMaterials);
	Model model(mesh);


	// Constuct model
	// Maybe make this a mesh, and have another class that constructs the model
	//Model<VertexPositionNormalTexture> model;

	//DX::ThrowIfFailed(model.Init(Direct3D::Get()->GetDevice().Get(), vertices, indices, texture));


	return model;
}


void OBJLoader::ReadLine(wstring line) {
	if (line[0] == OBJTokens::comment || line == L"") {
		return;
	}

	wchar_t* nextToken;
	wchar_t* cLine = _wcsdup(&line[0]);
	wchar_t* token = wcstok_s(cLine, L" ", &nextToken);

	line = wstring(nextToken);


	//----------------------------------------------------------------------------------
	// Model Info
	//----------------------------------------------------------------------------------

	// Vertex
	if (wcscmp(token, OBJTokens::vertex) == 0) {
		ReadVertex(line);
	}

	// Normal
	else if (wcscmp(token, OBJTokens::normal) == 0) {
		ReadNormal(line);
	}

	// Texture
	else if (wcscmp(token, OBJTokens::texture) == 0) {
		ReadTexCoord(line);
	}

	// Group
	else if (wcscmp(token, OBJTokens::group) == 0) {
		NewGroup();
	}

	// Face
	else if (wcscmp(token, OBJTokens::face) == 0) {
		ReadFace(line);
	}


	//----------------------------------------------------------------------------------
	// Material Info
	//----------------------------------------------------------------------------------

	// Material Library
	else if (wcscmp(token, OBJTokens::mtl_library) == 0) {
		ReadMaterialLibrary(line);
	}

	// Group Material
	else if (wcscmp(token, OBJTokens::group_mtl) == 0) {
		ReadMaterial(line);
	}

	// New Material
	else if (wcscmp(token, OBJTokens::new_mtl) == 0) {
		NewMaterial(line);
	}

	// Diffuse Color
	else if (wcscmp(token, OBJTokens::diffuse_color) == 0) {
		ReadDiffuse(line);
	}

	// Ambient Color
	else if (wcscmp(token, OBJTokens::ambient_color) == 0) {
		ReadAmbient(line);
	}

	// Dissolve (transparency)
	else if (wcscmp(token, OBJTokens::transparency) == 0) {
		ReadTransparency(line, false);
	}

	// Dissolve (transparency)
	else if (wcscmp(token, OBJTokens::transparency_inv) == 0) {
		ReadTransparency(line, true);
	}

	// Specular Color
	else if (wcscmp(token, OBJTokens::specular_color) == 0) {
		ReadSpecularColor(line);
	}

	// Specular Expononet
	else if (wcscmp(token, OBJTokens::specular_exponent) == 0) {
		ReadSpecularExp(line);
	}

	else if (wcscmp(token, OBJTokens::emmisive_color) == 0) {

	}

	// Optical Density
	else if (wcscmp(token, OBJTokens::optical_density) == 0) {
		ReadOpticalDensity(line);
	}

	// Illumination
	else if (wcscmp(token, OBJTokens::illumination_model) == 0) {
		ReadIllumination(line);
	}

	// Diffuse Map
	else if (wcscmp(token, OBJTokens::diffuse_color_map) == 0) {
		ReadDiffuseMap(line);
	}

	// Alpha Map
	else if (wcscmp(token, OBJTokens::alpha_texture_map) == 0) {
		ReadAlphaMap(line);
	}

	// Ambient Map
	else if (wcscmp(token, OBJTokens::ambient_color_map) == 0) {
		ReadAmbientMap(line);
	}

	// Specular Map
	else if (wcscmp(token, OBJTokens::specular_color_map) == 0) {
		ReadSpecularMap(line);
	}

	// Specular Highlight Map
	else if (wcscmp(token, OBJTokens::spec_highlight_map) == 0) {
		ReadSpecHighlightMap(line);
	}

	// Bump Map
	else if (wcscmp(token, OBJTokens::bump_map) == 0 || wcscmp(token, OBJTokens::bump_map2) == 0) {
		ReadBumpMap(line);
	}
}


void OBJLoader::ReadVertex(wstring& line) {
	wstringstream stream(line);

	XMFLOAT3 position;
	stream >> position.x >> position.y >> position.z;

	if (RHcoord) {
		position.z *= -1.0f;
	}

	vPositions.push_back(position);
}


void OBJLoader::ReadNormal(wstring& line) {
	wstringstream stream(line);

	XMFLOAT3 normal;
	stream >> normal.x >> normal.y >> normal.z;

	if (RHcoord) {
		normal.z *= -1.0f;
	}

	vNormals.push_back(normal);

	hasNormals = true;
}


void OBJLoader::ReadTexCoord(wstring& line) {
	wstringstream stream(line);

	XMFLOAT2 texCoord;
	stream >> texCoord.x >> texCoord.y;

	if (RHcoord) {
		texCoord.y = 1.0f - texCoord.y;
	}

	vTexCoords.push_back(texCoord);

	hasTexture = true;
}


void OBJLoader::NewGroup() {
	groupIndexStart.push_back(vIndex);
	++groupCount;
}


void OBJLoader::ReadFace(wstring& line) {
	wstringstream stream(line);
	wstring vDef;

	int vFirstIndex;
	int vLastIndex;
	int vPositionIndexTemp;
	int vNormalIndexTemp;
	int vTexCoordIndexTemp;

	triangleCount = std::count(line.begin(), line.end(), ' ') - 1;

	if (line.back() == ' ') {
		--triangleCount;
	}

	// First 3 vertices (first triangle)
	for (int i = 0; i < 3; ++i) {
		wstring vPart;
		int whichPart = 0;    //(position, texCoord, or normal)

		// Read one vertex definition
		stream >> vDef;

		// Parse the wstring
		for (int j = 0; j < vDef.length(); ++j) {
			// If there isn't a divider then add a char to vPart
			if (vDef[j] != L'/') {
				vPart += vDef[j];
			}

			// If the char is a divider or it's the end of the wstring
			if (vDef[j] == L'/' || j == vDef.length() - 1) {
				std::wstringstream wstringToInt(vPart);

				// If vPosition
				if (whichPart == 0) {
					wstringToInt >> vPositionIndexTemp;
					--vPositionIndexTemp;

					if (j == vDef.length() - 1) {
						vNormalIndexTemp = 0;
						vTexCoordIndexTemp = 0;
					}
				}

				// If vTexCoord
				else if (whichPart == 1) {
					// Check if there is a texCoord
					if (vPart != L"") {
						wstringToInt >> vTexCoordIndexTemp;
						vTexCoordIndexTemp--;
					}
					// If not, make a default
					else {
						vTexCoordIndexTemp = 0;
					}

					// If the char is the second to last, then there isn't a normal.
					// In this case, set a default.
					if (j == vDef.length() - 1) {
						vNormalIndexTemp = 0;
					}
				}

				// If vNormal
				else if (whichPart == 2) {
					wstringToInt >> vNormalIndexTemp;
					--vNormalIndexTemp;
				}

				// Set up for next vertex part
				vPart = L"";
				++whichPart;
			}
		}

		// Make sure there is at least one group
		if (groupCount == 0) {
			groupIndexStart.push_back(vIndex);
			++groupCount;
		}

		// Avoid duplicate vertices
		bool vExists = false;
		// Make sure there is at least one triangle to check
		if (vTotal >= 3) {

			// Loop through all the vertices
			for (int check = 0; check < vTotal; ++check) {

				// If the current vertex position and texCoord in memory are the same as
				// the vertex position and texCoord that were just from the file, then
				// set this face's vertex index to the value of the one in memory. This
				// ensures that no duplicates vertices are created.
				if (vPositionIndexTemp == vPositionIndices[check] && !vExists) {
					if (vTexCoordIndexTemp == vTexCoordIndices[check]) {

						// Set index for this vertex
						indices.push_back(check);
						// Vertex exists if this point was reaches
						vExists = true;
					}
				}
			}
		}

		// Add this vertex to the array if it isn't already
		if (!vExists) {
			vPositionIndices.push_back(vPositionIndexTemp);
			vTexCoordIndices.push_back(vTexCoordIndexTemp);
			vNormalIndices.push_back(vNormalIndexTemp);

			// Increment vertex count
			++vTotal;

			// Set index for this vertex
			indices.push_back(vTotal - 1);
		}

		// If this is the first vertex of this face, make sure the
		// rest of the triangles use this vertex
		if (i == 0) {
			vFirstIndex = indices[vIndex];
		}

		// If this is the last vertex in the first trangle, make sure
		// the next triangle uses this one (e.g. tri1(1,2,3) tri2(1,3,4) tri4(1,4,5))
		if (i == 2) {
			vLastIndex = indices[vIndex];
		}

		// Increment index count
		++vIndex;
	}

	// Increment triangle count
	++meshTriangles;


	// If there are more than three vertices in the face, the face needs to be
	// converted to triangles. The first triangle was created above, and a new
	// one will be created for every additional vertex using the first vertex
	// of the face and the last vertex from the previous triangle.
	for (int i = 0; i < triangleCount - 1; ++i) {

		// First vertex of this triangle (first vertex of the face)
		indices.push_back(vFirstIndex);
		++vIndex;

		// Second vertex of this triangle (last vertex of the previous triangle)
		indices.push_back(vLastIndex);
		++vIndex;

		// Get the third vertex
		stream >> vDef;

		wstring vPart;
		int whichPart = 0;

		// Parse the wstring (same as above)
		for (int j = 0; j < vDef.length(); ++j) {
			if (vDef[j] != L'/') {
				vPart += vDef[j];
			}

			if (vDef[j] == L'/' || j == vDef.length() - 1) {
				wstringstream wstringToInt(vPart);

				if (whichPart == 0) {
					wstringToInt >> vPositionIndexTemp;
					--vPositionIndexTemp;

					// Check if the vertex position was the only thing specified
					if (j == vDef.length() - 1) {
						vTexCoordIndexTemp = 0;
						vNormalIndexTemp = 0;
					}
				}

				else if (whichPart == 1) {
					if (vPart != L"") {
						wstringToInt >> vTexCoordIndexTemp;
						--vTexCoordIndexTemp;
					}

					else {
						vTexCoordIndexTemp = 0;
					}

					if (j == vDef.length() - 1) {
						vNormalIndexTemp = 0;
					}
				}

				else if (whichPart == 2) {
					wstringToInt >> vNormalIndexTemp;
					--vNormalIndexTemp;
				}

				vPart = L"";
				++whichPart;
			}
		}

		// Check for duplicate vertices
		bool vExists = false;
		// Make sure there is at least one triangle to check
		if (vTotal >= 3) {
			for (int check = 0; check < vTotal; ++check) {
				if (vPositionIndexTemp == vPositionIndices[check] && !vExists) {
					if (vTexCoordIndexTemp == vTexCoordIndices[check]) {
						indices.push_back(check);
						vExists = true;
					}
				}
			}
		}

		if (!vExists) {
			vPositionIndices.push_back(vPositionIndexTemp);
			vTexCoordIndices.push_back(vTexCoordIndexTemp);
			vNormalIndices.push_back(vNormalIndexTemp);

			++vTotal;
			indices.push_back(vTotal - 1);
		}

		// Set the second vertex for the next triangle
		vLastIndex = indices[vIndex];

		// Increment triangle count
		++meshTriangles;
		++vIndex;
	}
}


void OBJLoader::ReadMaterialLibrary(wstring& line) {
	wstring name = TrimWhiteSpace(line);

	meshMatLib = name;
}


void OBJLoader::NewMaterial(wstring& line) {
	wstring name = TrimWhiteSpace(line);

	OBJMaterial temp;
	temp.name = name;
	materials.push_back(temp);

	++materialCount;
	kdSet = false;
}


void OBJLoader::ReadMaterial(wstring& line) {
	wstring name = TrimWhiteSpace(line);

	meshMaterials[groupCount - 1] = name;
}


void OBJLoader::ReadDiffuse(wstring& line) {
	wstringstream stream(line);

	stream >> materials[materialCount - 1].Kd.x;
	stream >> materials[materialCount - 1].Kd.y;
	stream >> materials[materialCount - 1].Kd.z;

	kdSet = true;
}


void OBJLoader::ReadAmbient(wstring& line) {
	wstringstream stream(line);

	if (!kdSet) {
		stream >> materials[materialCount - 1].Kd.x;
		stream >> materials[materialCount - 1].Kd.y;
		stream >> materials[materialCount - 1].Kd.z;
	}

	stream >> materials[materialCount - 1].Ka.x;
	stream >> materials[materialCount - 1].Ka.y;
	stream >> materials[materialCount - 1].Ka.z;
}


void OBJLoader::ReadTransparency(wstring& line, bool inverse) {
	wstringstream stream(line);

	float transparency;
	stream >> transparency;

	if (inverse) {
		transparency = 1.0f - transparency;
	}

	materials[materialCount - 1].d = transparency;

	if (transparency > 0.0f) {
		materials[materialCount - 1].transparency = true;
	}
}


void OBJLoader::ReadSpecularColor(wstring& line) {
	wstringstream stream(line);

	stream >> materials[materialCount - 1].Ks.x;
	stream >> materials[materialCount - 1].Ks.y;
	stream >> materials[materialCount - 1].Ks.z;
}


void OBJLoader::ReadSpecularExp(wstring& line) {
	wstringstream stream(line);

	stream >> materials[materialCount - 1].Ns;
}


void OBJLoader::ReadOpticalDensity(wstring& line) {
	wstringstream stream(line);

	stream >> materials[materialCount - 1].Ni;
}


void OBJLoader::ReadIllumination(wstring& line) {
	wstringstream stream(line);

	stream >> materials[materialCount - 1].illum;
}


void OBJLoader::ReadDiffuseMap(wstring& line) {
	wstring filename = TrimWhiteSpace(line);

	// CREATE TEXTURE WITH TEXTUREMGR

	materials[materialCount - 1].map_Kd = filename;
	//materials[materialCount - 1].hasTexture = true; 
}


void OBJLoader::ReadAlphaMap(wstring& line) {
	wstring filename = TrimWhiteSpace(line);

	// CREATE TEXTURE WITH TEXTUREMGR

	materials[materialCount - 1].map_d = filename;
	materials[materialCount - 1].transparency = true;
}


void OBJLoader::ReadAmbientMap(wstring& line) {
	wstring filename = TrimWhiteSpace(line);

	materials[materialCount - 1].map_Ka = filename;
}


void OBJLoader::ReadSpecularMap(wstring& line) {
	wstring filename = TrimWhiteSpace(line);

	materials[materialCount - 1].map_Ks = filename;
}


void OBJLoader::ReadSpecHighlightMap(wstring& line) {
	wstring filename = TrimWhiteSpace(line);

	materials[materialCount - 1].map_Ns = filename;
}


void OBJLoader::ReadBumpMap(wstring& line) {
	wstring filename = TrimWhiteSpace(line);

	materials[materialCount - 1].map_bump = filename;
}

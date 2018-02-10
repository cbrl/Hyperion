#include "stdafx.h"
#include "OBJLoader.h"
#include "loader\OBJTokens.h"


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


/*Model<VertexPositionNormalTexture>*/ void OBJLoader::Load(wstring filename, bool RHcoordinates) {
	RHcoord = RHcoordinates;

	wstring line;

	// Open OBJ file
	wifstream file(filename);
	if (file.fail()) {
		throw std::exception("Could not open obj file");
	}

	// Read file contents
	while (std::getline(file, line)) {
		ReadLine(line);
	}

	// Close the OBJ file
	file.close();

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



	// Open material file
	file.open(meshMatLib);
	materialCount = materials.size();
	
	while (std::getline(file, line)) {
		ReadLine(line);
	}



	// Constuct model
	// Maybe make this a mesh, and have another class that constructs the model
	//Model<VertexPositionNormalTexture> model;

	//DX::ThrowIfFailed(model.Init(Direct3D::Get()->GetDevice().Get(), vertices, indices, texture));


	//return model;
}


void OBJLoader::ReadLine(wstring line) {
	if (line[0] == OBJTokens::comment || line == L"") {
		return;
	}

	wchar_t* nextToken;
	wchar_t* cLine = _wcsdup(&line[0]);
	wchar_t* token = wcstok_s(cLine, L" ", &nextToken);

	line = wstring(nextToken);

	if (wcscmp(token, OBJTokens::vertex) == 0) {
		ReadVertex(line);
	}

	else if (wcscmp(token, OBJTokens::normal) == 0) {
		ReadNormal(line);
	}

	else if (wcscmp(token, OBJTokens::texture) == 0) {
		ReadTexCoord(line);
	}

	else if (wcscmp(token, OBJTokens::group) == 0) {
		NewGroup();
	}

	else if (wcscmp(token, OBJTokens::face) == 0) {
		ReadFace(line);
	}

	else if (wcscmp(token, OBJTokens::mtl_library) == 0) {
		ReadMaterialLibrary(line);
	}

	else if (wcscmp(token, OBJTokens::group_mtl) == 0) {
		ReadMaterial(line);
	}

	else if (wcscmp(token, OBJTokens::diffuse_color) == 0) {
		ReadDiffuse(line);
	}

	else if (wcscmp(token, OBJTokens::ambient_color) == 0) {
		ReadAmbient(line);
	}

	else if (wcscmp(token, OBJTokens::transparency) == 0) {
		ReadTransparency(line, false);
	}

	else if (wcscmp(token, OBJTokens::transparency_inv) == 0) {
		ReadTransparency(line, true);
	}

	else if (wcscmp(token, OBJTokens::diffuse_color_map) == 0) {
		ReadDiffuseMap(line);
	}

	else if (wcscmp(token, OBJTokens::new_mtl) == 0) {
		NewMaterial(line);
	}
}


void OBJLoader::ReadVertex(wstring line) {
	wstringstream stream(line);

	XMFLOAT3 position;
	stream >> position.x >> position.y >> position.z;

	if (RHcoord) {
		position.z *= -1.0f;
	}

	vPositions.push_back(position);
}


void OBJLoader::ReadNormal(wstring line) {
	wstringstream stream(line);

	XMFLOAT3 normal;
	stream >> normal.x >> normal.y >> normal.z;

	if (RHcoord) {
		normal.z *= -1.0f;
	}

	vNormals.push_back(normal);

	hasNormals = true;
}


void OBJLoader::ReadTexCoord(wstring line) {
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
	groupCount++;
}


void OBJLoader::ReadFace(wstring line) {
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

		// Parse the string
		for (int j = 0; j < vDef.length(); ++j) {
			// If there isn't a divider then add a char to vPart
			if (vDef[j] != '/') {
				vPart += vDef[j];
			}

			// If the char is a divider or it's the end of the string
			if (vDef[j] == '/' || j == vDef.length() - 1) {
				std::wistringstream wstringToInt(vPart);

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
				if (vPositionIndexTemp == vPositionIndex[check] && !vExists) {
					if (vTexCoordIndexTemp == vTexCoordIndex[check]) {

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
			vPositionIndex.push_back(vPositionIndexTemp);
			vTexCoordIndex.push_back(vTexCoordIndexTemp);
			vNormalIndex.push_back(vNormalIndexTemp);

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

		// Parse the string (same as above)
		for (int j = 0; j < vDef.length(); ++j) {
			if (vDef[j] != '/') {
				vPart += vDef[j];
			}

			if (vDef[j] == '/' || j == vDef.length() - 1) {
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
				if (vPositionIndexTemp == vPositionIndex[check] && !vExists) {
					if (vTexCoordIndexTemp == vTexCoordIndex[check]) {
						indices.push_back(check);
						vExists = true;
					}
				}
			}
		}

		if (!vExists) {
			vPositionIndex.push_back(vPositionIndexTemp);
			vTexCoordIndex.push_back(vTexCoordIndexTemp);
			vNormalIndex.push_back(vNormalIndexTemp);

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


void OBJLoader::ReadMaterialLibrary(wstring line) {
	wstringstream stream(line);

	stream >> meshMatLib;
}


void OBJLoader::ReadMaterial(wstring line) {
	wstringstream stream(line);

	wstring temp;
	stream >> temp;

	meshMaterials.push_back(temp);
}


void OBJLoader::ReadDiffuse(wstring line) {
	wstringstream stream(line);

	stream >> materials[materialCount - 1].diffuseColor.x;
	stream >> materials[materialCount - 1].diffuseColor.y;
	stream >> materials[materialCount - 1].diffuseColor.z;

	kdSet = true;
}


void OBJLoader::ReadAmbient(wstring line) {
	wstringstream stream(line);

	if (!kdSet) {
		stream >> materials[materialCount - 1].diffuseColor.x;
		stream >> materials[materialCount - 1].diffuseColor.y;
		stream >> materials[materialCount - 1].diffuseColor.z;
	}
}


void OBJLoader::ReadTransparency(wstring line, bool inverse) {
	wstringstream stream(line);

	float transparency;
	stream >> transparency;

	if (inverse) {
		transparency = 1.0f - transparency;
	}

	materials[materialCount - 1].diffuseColor.w = transparency;

	if (transparency > 0.0f) {
		materials[materialCount - 1].transparent = true;
	}
}


void OBJLoader::NewMaterial(wstring line) {
	wstringstream stream(line);

	Material temp;
	stream >> temp.name;
	materials.push_back(temp);

	++materialCount;
	kdSet = false;
}


void OBJLoader::ReadDiffuseMap(wstring line) {
	wstringstream stream(line);

	wstring filename;
	stream >> filename;

	// CREATE TEXTURE WITH TEXTUREMGR

	materials[materialCount - 1].diffuseMap = filename;
	materials[materialCount - 1].hasTexture = true;
}


void OBJLoader::ReadAlphaMap(wstring line) {
	wstringstream stream(line);

	wstring filename;
	stream >> filename;

	// CREATE TEXTURE WITH TEXTUREMGR

	materials[materialCount - 1].alphaMap = filename;
	materials[materialCount - 1].transparent = true;
}

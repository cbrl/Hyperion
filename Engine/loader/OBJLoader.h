#pragma once

#include <fstream>
#include <string>
#include <cwchar>
#include <vector>
#include <algorithm>
#include <d3d11.h>
#include <VertexTypes.h>

#include "direct3d\direct3d.h"
//#include "geometry\model\model2.h"
#include "geometry\mesh\mesh.h"

using std::wstring;
using std::vector;
using std::wifstream;
using std::wstringstream;

class OBJLoader {
	public:
		OBJLoader();
		~OBJLoader();

		//Model<VertexPositionNormalTexture> Load(wstring filename, bool RHcoordinates);
		void Load(wstring filename, bool RHcoordinates);

	private:
		void ReadLine(wstring line);
		void ReadVertex(wstring line);
		void ReadNormal(wstring line);
		void ReadTexCoord(wstring line);
		void NewGroup();
		void ReadFace(wstring line);
		void ReadMaterialLibrary(wstring line);
		void ReadMaterial(wstring line);


	private:
		struct Material {
			wstring name;
			XMFLOAT4 diffuseColor;
			int texArrayIndex;
			bool hasTexture;
			bool transparent;
		};


	private:
		bool RHcoord;
		bool hasNormals;
		bool hasTexture;

		int vIndex;
		int vTotal;
		int triangleCount;
		int meshTriangles;

		vector<XMFLOAT3> vPositions;
		vector<XMFLOAT3> vNormals;
		vector<XMFLOAT2> vTexCoords;
		vector<ULONG>    indices;

		vector<int> vPositionIndex;
		vector<int> vNormalIndex;
		vector<int> vTexCoordIndex;

		vector<int> groupIndexStart;
		int         groupCount;

		wstring         meshMatLib;
		vector<wstring> meshMaterials;

		vector<Material> materials;
};
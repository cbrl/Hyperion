#pragma once

#include <fstream>
#include <string>
#include <cwchar>
#include <vector>
#include <algorithm>
#include <d3d11.h>
#include <DirectXMath.h>
#include <VertexTypes.h>

#include "direct3d\Direct3d.h"
#include "geometry\model\Model2.h"
#include "geometry\mesh\Mesh.h"
#include "material\Material.h"

using std::wstring;
using std::vector;
using std::wifstream;
using std::wstringstream;

using namespace DirectX;

class OBJLoader {
	public:
		OBJLoader();
		~OBJLoader();

		Model Load(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring folder, wstring filename, bool RHcoordinates);

	private:
		wstring TrimWhiteSpace(wstring& in);
		void ReadLine(wstring line);

		// Model Info
		void ReadVertex(wstring& line);
		void ReadNormal(wstring& line);
		void ReadTexCoord(wstring& line);
		void NewGroup();
		void ReadFace(wstring& line);

		// Material Info
		void ReadMaterialLibrary(wstring& line);
		void ReadMaterial(wstring& line);
		void ReadDiffuse(wstring& line);
		void ReadAmbient(wstring& line);
		void ReadTransparency(wstring& line, bool inverse);
		void ReadSpecularColor(wstring& line);
		void ReadSpecularExp(wstring& line);
		void ReadOpticalDensity(wstring& line);
		void ReadIllumination(wstring& line);
		void NewMaterial(wstring& line);
		void ReadDiffuseMap(wstring& line);
		void ReadAlphaMap(wstring& line);
		void ReadAmbientMap(wstring& line);
		void ReadSpecularMap(wstring& line);
		void ReadSpecHighlightMap(wstring& line);
		void ReadBumpMap(wstring& line);
		

	private:
		struct OBJMaterial {
			OBJMaterial() {
				Ns = 0.0f;
				Ni = 0.0f;
				d = 0.0f;
				illum = 0;
				transparency = false;
			}

			wstring name;
			// Ambient map
			wstring map_Ka;
			// Diffuse map
			wstring map_Kd;
			// Specular map
			wstring map_Ks;
			// Specular Highlight map
			wstring map_Ns;
			// Alpha map
			wstring map_d;
			// Bump map
			wstring map_bump;
			// Ambient Color
			XMFLOAT3 Ka;
			// Diffuse Color
			XMFLOAT3 Kd;
			// Specular Color
			XMFLOAT3 Ks;
			// Specular Exponent
			float Ns;
			// Optical Density
			float Ni;
			// Dissolve
			float d;
			// Illumination
			int illum;

			// Transparency flag
			bool transparency;
		};


	private:
		bool RHcoord;
		bool hasNormals;
		bool hasTexture;
		// If diffuse color wasn't set, the ambient color can be used (usually the same)
		bool kdSet;

		int vIndex;
		int vTotal;
		int triangleCount;
		int meshTriangles;
		int materialCount;

		// Vectors to store model info
		vector<XMFLOAT3> vPositions;
		vector<XMFLOAT3> vNormals;
		vector<XMFLOAT2> vTexCoords;
		vector<ULONG>    indices;

		// Vectors to store indices for vertex definitions
		vector<UINT> vPositionIndices;
		vector<UINT> vNormalIndices;
		vector<UINT> vTexCoordIndices;

		// Index values of the start of new groups
		vector<UINT> groupIndexStart;
		int groupCount;

		vector<UINT> groupMaterials;

		// Material library name
		wstring meshMatLib;
		// Material names for each group
		std::map<int, wstring> meshMaterials;

		// Vector of material descriptions
		vector<OBJMaterial> materials;
};
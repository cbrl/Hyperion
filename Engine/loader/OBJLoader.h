#pragma once

#include <fstream>
#include <string>
#include <cwchar>
#include <vector>
#include <algorithm>
#include <d3d11.h>
#include <DirectXMath.h>
#include <VertexTypes.h>

#include "util\math\math.h"
#include "util\math\directxmath\extensions.h"
#include "util\string\string.h"
#include "direct3d\Direct3d.h"
#include "geometry\model\Model2.h"
#include "geometry\mesh\Mesh.h"
#include "material\Material.h"

using std::stoi;
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
		template<typename ElementT>
		const ElementT& GetElement(vector<ElementT>& in, int index);

		void LoadModel(wstring folder, wstring filename);
		void LoadMaterials(wstring folder);

		void ReadFace(wstring& line);
		void Triangulate(vector<VertexPositionNormalTexture>& inVerts, vector<UINT>& outIndices);
		void ReadTransparency(wstring& line, bool inverse);
		

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

		int groupCount;
		int mtlCount;

		// Vector of complete vertex definitions
		vector<VertexPositionNormalTexture> vertices;

		// Vectors to store model info
		vector<XMFLOAT3> vPositions;
		vector<XMFLOAT3> vNormals;
		vector<XMFLOAT2> vTexCoords;
		vector<UINT>    indices;

		// Material library name
		wstring meshMatLib;

		// Material names for each group <grp number, mat name>
		std::map<int, wstring> meshMaterials;

		// Vector of material descriptions
		vector<OBJMaterial> materials;

		// List of vertices where a new group starts
		vector<UINT> groupVertexIndices;

		// Index values of the material for each group
		vector<UINT> groupMaterialIndices;
};


template<typename ElementT>
inline const ElementT& OBJLoader::GetElement(vector<ElementT>& in, int index) {
	if (index < 0) {
		index = in.size() + index;
	}

	return in[index];
}
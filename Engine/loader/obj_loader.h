#pragma once

#include <fstream>
#include <string>
#include <cwchar>
#include <vector>
#include <map>
#include <algorithm>
#include <d3d11.h>
#include <DirectXMath.h>

#include "util\math\math.h"
#include "geometry\mesh\vertex_types.h"
#include "util\string\string.h"
#include "texture\texture_mgr.h"
#include "geometry\model\model.h"
#include "geometry\mesh\mesh.h"
#include "geometry\boundingvolume\bounding_volume.h"
#include "material\material.h"

using std::stoi;
using std::wstring;
using std::map;
using std::vector;
using std::wifstream;
using std::wstringstream;

using namespace DirectX;

class OBJLoader {
	public:
		OBJLoader();
		~OBJLoader();

		Model Load(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring folder, wstring filename, bool RHcoordinates);

	private:
		template<typename ElementT>
		const ElementT& GetElement(vector<ElementT>& in, size_t index) {
			if (index < 0) {
				index = in.size() + index;
			}
			return in[index];
		}

		void Reset();

		void LoadModel(wstring folder, wstring filename);
		void LoadMaterials(wstring folder);

		void ReadFace(wstring& line);
		void Triangulate(vector<VertexPositionNormalTexture>& inVerts, vector<UINT>& outIndices);
		void ReadTransparency(wstring& line, bool inverse);
		

	private:
		struct OBJMaterial {
			OBJMaterial() :
				Ka(0.0f, 0.0f, 0.0f, 1.0f),
				Kd(0.0f, 0.0f, 0.0f, 1.0f),
				Ks(0.0f, 0.0f, 0.0f, 1.0f),
				Ke(0.0f, 0.0f, 0.0f, 1.0f),
				Ns(0.0f),
				Ni(0.0f),
				d(0.0f),
				illum(0),
				transparency(false) {}

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
			XMFLOAT4 Ka;
			// Diffuse Color
			XMFLOAT4 Kd;
			// Specular Color
			XMFLOAT4 Ks;
			// Emissive Color
			XMFLOAT4 Ke;
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
		bool RH_coord;
		int  group_count;
		int  mtl_count;

		// Vector of complete vertex definitions
		vector<VertexPositionNormalTexture> vertices;

		// Vectors to store model info
		vector<XMFLOAT3> vertex_positions;
		vector<XMFLOAT3> vertex_normals;
		vector<XMFLOAT2> vertex_texCoords;
		vector<UINT>     indices;

		// Material library name
		wstring meshMatLib;

		// Material names for each group <grp number, mat name>
		map<int, wstring> group_materials;

		// Vector of material descriptions
		vector<OBJMaterial> materials;

		// List of indices where a new group starts (e.g. 8 = new group at indices[8])
		vector<UINT> new_group_indices;

		// List of materials for each group. Value is an index for the material vector.
		vector<UINT> group_material_indices;
};
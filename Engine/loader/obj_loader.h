#pragma once

#include <algorithm>
#include <d3d11.h>

#include "util\io\io.h"
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "resource\model\model_blueprint.h"
#include "resource\model\model.h"
#include "resource\material\material.h"


class ResourceMgr;


class OBJLoader {
	public:
		OBJLoader();
		~OBJLoader();

		Model Load(ID3D11Device* device,
		           ResourceMgr& resource_mgr,
				   wstring folder, wstring filename,
				   bool right_hand_coords);


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
		void Triangulate(vector<VertexPositionNormalTexture>& inVerts, vector<u32>& outIndices);
		void ReadTransparency(wstring& line, bool inverse);
		

	private:
		struct OBJMaterial {
			OBJMaterial() :
				Ka(0.0f, 0.0f, 0.0f, 1.0f),
				Kd(0.0f, 0.0f, 0.0f, 1.0f),
				Ks(0.0f, 0.0f, 0.0f, 1.0f),
				Ke(0.0f, 0.0f, 0.0f, 1.0f),
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
			float4 Ka;
			// Diffuse Color
			float4 Kd;
			// Specular Color, w = spec exponent
			float4 Ks;
			// Emissive Color
			float4 Ke;
			// Optical Density
			float Ni;
			// Dissolve
			float d;
			// Illumination
			i32 illum;

			// Transparency flag
			bool transparency;
		};


	private:
		bool rh_coord;
		i32  group_count;
		i32  mtl_count;

		// Vector of complete vertex definitions
		vector<VertexPositionNormalTexture> vertices;

		// Vectors to store model info
		vector<float3> vertex_positions;
		vector<float3> vertex_normals;
		vector<float2> vertex_texCoords;
		vector<u32>    indices;

		// Material library name
		wstring meshMatLib;

		// Material names for each group <grp number, mat name>
		map<u32, wstring> group_materials;

		// Vector of material descriptions
		vector<OBJMaterial> materials;

		// List of indices where a new group starts (e.g. 8 = new group at indices[8])
		vector<u32> new_group_indices;

		// List of materials for each group. Value is an index for the material vector.
		vector<u32> group_material_indices;
};
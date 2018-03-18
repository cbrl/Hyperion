#pragma once

#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "resource\resource_mgr.h"
#include "resource\model\model_blueprint.h"
#include "resource\material\material.h"


class OBJLoader {
	public:
		OBJLoader() = delete;

		static void Load(ID3D11Device* device,
						 ResourceMgr& resource_mgr,
						 const wstring& folder,
						 const wstring& filename,
						 bool right_hand_coords,
						 ModelBlueprint& blueprint_out);


	private:
		template<typename ElementT>
		static const ElementT& GetElement(vector<ElementT>& in, size_t index) {
			if (index < 0) {
				index = in.size() + index;
			}
			return in[index];
		}

		static void Reset();

		static void LoadModel(wstring folder, wstring filename);
		static void LoadMaterials(wstring folder);

		static void ReadFace(wstring& line);
		static void Triangulate(vector<VertexPositionNormalTexture>& inVerts, vector<u32>& outIndices);
		static void ReadTransparency(wstring& line, bool inverse);
		

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
		static bool rh_coord;
		static u32  group_count;
		static u32  mtl_count;

		// Vector of complete vertex definitions
		static vector<VertexPositionNormalTexture> vertices;

		// Vectors for position/normal/texCoord/index
		static vector<float3> vertex_positions;
		static vector<float3> vertex_normals;
		static vector<float2> vertex_texCoords;
		static vector<u32>    indices;

		// Material library name
		static wstring mat_lib;

		// Vector of group definitions
		static vector<Group> groups;

		// Material names for each group <grp number, mat name>
		static map<u32, wstring> group_mat_names;

		// Vector of material definitions
		static vector<OBJMaterial> materials;
};



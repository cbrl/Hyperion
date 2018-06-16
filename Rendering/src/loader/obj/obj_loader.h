#pragma once

#include "directx/d3d11.h"

#include "datatypes/datatypes.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"
#include "resource/model/material/material.h"


// Material definition used by OBJLoader
struct ObjMaterial {
	ObjMaterial() noexcept
		: Ka(0.0f, 0.0f, 0.0f, 1.0f)
		, Kd(0.0f, 0.0f, 0.0f, 1.0f)
		, Ks(0.0f, 0.0f, 0.0f, 1.0f)
		, Ke(0.0f, 0.0f, 0.0f, 1.0f)
		, Ni(0.0f)
		, d(0.0f)
		, illum(0)
		, transparency(false) {
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


template<typename VertexT>
class ObjLoader final {
public:
	ObjLoader() = delete;

	static ModelOutput<VertexT> load(ResourceMgr& resource_mgr,
	                                 const wstring& filename,
	                                 bool right_hand_coords);


private:
	static void reset();

	static void loadModel(wstring filename);
	static void loadMaterials(wstring folder);

	static void readFace(wstring& line);
	static void triangulate(vector<VertexT>& in_verts, vector<u32>& out_indices);
	static void readTransparency(wstring& line, bool inverse);


private:
	static bool rh_coord;
	static u32 group_count;
	static u32 mtl_count;

	// Vector of complete vertex definitions
	static vector<VertexT> vertices;

	// Vectors for position/normal/texCoord/index
	static vector<float3> vertex_positions;
	static vector<float3> vertex_normals;
	static vector<float2> vertex_texCoords;
	static vector<u32> indices;

	// Material library name
	static wstring mat_lib;

	// Vector of group definitions
	static vector<Group> groups;

	// Material names for each group <grp number, mat name>
	static map<u32, wstring> group_mat_names;

	// Vector of material definitions
	static vector<ObjMaterial> materials;
};


//----------------------------------------------------------------------------------
// Definitions of static members
//----------------------------------------------------------------------------------

template<typename VertexT>
bool ObjLoader<VertexT>::rh_coord = false;

template<typename VertexT>
u32 ObjLoader<VertexT>::group_count = 0;

template<typename VertexT>
u32 ObjLoader<VertexT>::mtl_count = 0;

template<typename VertexT>
vector<VertexT> ObjLoader<VertexT>::vertices;

template<typename VertexT>
vector<float3> ObjLoader<VertexT>::vertex_positions;

template<typename VertexT>
vector<float3> ObjLoader<VertexT>::vertex_normals;

template<typename VertexT>
vector<float2> ObjLoader<VertexT>::vertex_texCoords;

template<typename VertexT>
vector<u32> ObjLoader<VertexT>::indices;

template<typename VertexT>
wstring ObjLoader<VertexT>::mat_lib;

template<typename VertexT>
vector<Group> ObjLoader<VertexT>::groups;

template<typename VertexT>
map<u32, wstring> ObjLoader<VertexT>::group_mat_names;

template<typename VertexT>
vector<ObjMaterial> ObjLoader<VertexT>::materials;


#include "obj_loader.tpp"

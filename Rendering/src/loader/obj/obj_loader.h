#pragma once

#include "directx/d3d11.h"

#include "datatypes/datatypes.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"
#include "resource/model/material/material.h"


template<typename VertexT>
class OBJLoader final {
public:
	OBJLoader() = delete;
	OBJLoader(const OBJLoader& loader) = delete;
	OBJLoader(OBJLoader&& loader) = delete;

	[[nodiscard]]
	static ModelOutput<VertexT> load(ResourceMgr& resource_mgr,
	                                 const wstring& filename,
	                                 bool right_hand_coords);


private:
	struct OBJMaterial {
		OBJMaterial() noexcept
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
		vec4_f32 Ka;
		// Diffuse Color
		vec4_f32 Kd;
		// Specular Color, w = spec exponent
		vec4_f32 Ks;
		// Emissive Color
		vec4_f32 Ke;
		// Optical Density
		f32 Ni;
		// Dissolve
		f32 d;
		// Illumination
		i32 illum;

		// Transparency flag
		bool transparency;
	};


	struct OBJLess final {
		bool operator()(vec3_u32 left, vec3_u32 right) const noexcept {
			return left.x == right.x ? left.y == right.y ? left.z < right.z
			                                             : left.y < right.y
			                         : left.x < right.x;
		}
	};


private:
	static void reset();


	static void loadModel(wstring filename);
	static void readFace(wstring& line);
	static VertexT createVertex(vec3_u32 vert_def);
	static void triangulate(vector<vec3_u32>& face_def);

	static void loadMaterials(wstring folder);
	static void readTransparency(wstring& line, bool inverse);


private:
	static bool rh_coord;

	// Vector of group definitions
	static vector<Group> groups;

	// Vector of complete vertex definitions
	static vector<VertexT> vertices;

	// Maps face definitions to an index
	static map<vec3_u32, u32, OBJLess> index_map;

	// Vectors for position/normal/texCoord/index
	static vector<vec3_f32> vertex_positions;
	static vector<vec3_f32> vertex_normals;
	static vector<vec2_f32> vertex_texCoords;
	static vector<u32> indices;

	// Material library name
	static wstring mat_lib;

	// Material names for each group <grp number, mat name>
	static map<u32, wstring> group_mat_names;

	// Vector of material definitions
	static vector<OBJMaterial> materials;
};


//----------------------------------------------------------------------------------
// Definitions of static members
//----------------------------------------------------------------------------------

template<typename VertexT>
bool OBJLoader<VertexT>::rh_coord = false;

template<typename VertexT>
vector<VertexT> OBJLoader<VertexT>::vertices;

template<typename VertexT>
vector<vec3_f32> OBJLoader<VertexT>::vertex_positions;

template<typename VertexT>
vector<vec3_f32> OBJLoader<VertexT>::vertex_normals;

template<typename VertexT>
vector<vec2_f32> OBJLoader<VertexT>::vertex_texCoords;

template<typename VertexT>
map<vec3_u32, u32, typename OBJLoader<VertexT>::OBJLess> OBJLoader<VertexT>::index_map;

template<typename VertexT>
vector<u32> OBJLoader<VertexT>::indices;

template<typename VertexT>
wstring OBJLoader<VertexT>::mat_lib;

template<typename VertexT>
vector<Group> OBJLoader<VertexT>::groups;

template<typename VertexT>
map<u32, wstring> OBJLoader<VertexT>::group_mat_names;

template<typename VertexT>
vector<typename OBJLoader<VertexT>::OBJMaterial> OBJLoader<VertexT>::materials;


#include "obj_loader.tpp"

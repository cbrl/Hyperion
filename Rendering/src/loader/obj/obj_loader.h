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
	                                 const fs::path& filename,
	                                 bool right_hand_coords);


private:
	struct OBJMaterial {
		OBJMaterial() noexcept
			: Ka(0.0f, 0.0f, 0.0f, 1.0f)
			, Kd(0.0f, 0.0f, 0.0f, 1.0f)
			, Ks(0.0f, 0.0f, 0.0f, 1.0f)
			, Ke(0.0f, 0.0f, 0.0f, 1.0f)
			, Ni(0.0f)
			, d(1.0f)
			, illum(0)
			, transparency(false) {
		}

		std::string name;
		// Ambient std::map
		std::string map_Ka;
		// Diffuse std::map
		std::string map_Kd;
		// Specular std::map
		std::string map_Ks;
		// Specular Highlight std::map
		std::string map_Ns;
		// Alpha std::map
		std::string map_d;
		// Bump std::map
		std::string map_bump;
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

	static void loadModel(const fs::path& file);
	static void readFace(const std::string& line);
	static VertexT createVertex(vec3_u32 vert_def);
	static void triangulate(std::vector<vec3_u32>& face_def);

	static void loadMaterials(const fs::path& mat_file);


private:
	static bool rh_coord;

	// Vector of group definitions
	static std::vector<Group> groups;

	// Vector of complete vertex definitions
	static std::vector<VertexT> vertices;

	// Maps face definitions to an index
	static std::map<vec3_u32, u32, OBJLess> index_map;

	// Vectors for position/normal/texCoord/index
	static std::vector<vec3_f32> vertex_positions;
	static std::vector<vec3_f32> vertex_normals;
	static std::vector<vec2_f32> vertex_texCoords;
	static std::vector<u32> indices;

	// Material library name
	static std::string mat_lib;

	// Material names for each group <grp number, mat name>
	static std::map<u32, std::string> group_mat_names;

	// Vector of material definitions
	static std::vector<OBJMaterial> materials;
};


//----------------------------------------------------------------------------------
// Definitions of static members
//----------------------------------------------------------------------------------

template<typename VertexT>
bool OBJLoader<VertexT>::rh_coord = false;

template<typename VertexT>
std::vector<VertexT> OBJLoader<VertexT>::vertices;

template<typename VertexT>
std::vector<vec3_f32> OBJLoader<VertexT>::vertex_positions;

template<typename VertexT>
std::vector<vec3_f32> OBJLoader<VertexT>::vertex_normals;

template<typename VertexT>
std::vector<vec2_f32> OBJLoader<VertexT>::vertex_texCoords;

template<typename VertexT>
std::map<vec3_u32, u32, typename OBJLoader<VertexT>::OBJLess> OBJLoader<VertexT>::index_map;

template<typename VertexT>
std::vector<u32> OBJLoader<VertexT>::indices;

template<typename VertexT>
std::string OBJLoader<VertexT>::mat_lib;

template<typename VertexT>
std::vector<Group> OBJLoader<VertexT>::groups;

template<typename VertexT>
std::map<u32, std::string> OBJLoader<VertexT>::group_mat_names;

template<typename VertexT>
std::vector<typename OBJLoader<VertexT>::OBJMaterial> OBJLoader<VertexT>::materials;


#include "obj_loader.tpp"

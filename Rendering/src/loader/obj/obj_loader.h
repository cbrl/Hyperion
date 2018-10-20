#pragma once

#include "datatypes/datatypes.h"
#include "io/line_reader.h"
#include "resource/model/model_output.h"
#include "resource/model/material/material.h"

class ResourceMgr;

template<typename VertexT>
class ObjLoader final : public LineReader {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ObjLoader(ResourceMgr& resource_mgr);
	ObjLoader(const ObjLoader& loader) = delete;
	ObjLoader(ObjLoader&& loader) = delete;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ObjLoader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ObjLoader& operator=(const ObjLoader& loader) = delete;
	ObjLoader& operator=(ObjLoader&& loader) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions - Load obj file
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	ModelOutput<VertexT> load(const fs::path& file, bool right_hand_coords);


private:
	//----------------------------------------------------------------------------------
	// Comparison struct for index map
	//----------------------------------------------------------------------------------
	struct OBJLess final {
		bool operator()(vec3_u32 left, vec3_u32 right) const noexcept {
			return left.x == right.x ? left.y == right.y ? left.z < right.z
			                                             : left.y < right.y
			                         : left.x < right.x;
		}
	};


private:
	//----------------------------------------------------------------------------------
	// Member Functions - Read file
	//----------------------------------------------------------------------------------
	void readLine() override;
	void loadMaterials(const fs::path& mat_file);


	//----------------------------------------------------------------------------------
	// Member Functions - Create Face
	//----------------------------------------------------------------------------------
	void readFace();
	VertexT createVertex(vec3_u32 vert_def);
	void triangulate(std::vector<vec3_u32>& face_def);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ResourceMgr& resource_mgr;
	bool rh_coord;


	//----------------------------------------------------------------------------------
	// Member Variables - Model Definition
	//----------------------------------------------------------------------------------

	// Vector of group definitions
	std::vector<Group> groups;

	// Vector of complete vertex definitions
	std::vector<VertexT> vertices;

	// Maps face definitions to an index
	std::map<vec3_u32, u32, OBJLess> index_map;

	// Vectors for position/normal/texCoord/index
	std::vector<vec3_f32> vertex_positions;
	std::vector<vec3_f32> vertex_normals;
	std::vector<vec2_f32> vertex_texCoords;
	std::vector<u32> indices;


	//----------------------------------------------------------------------------------
	// Member Variables - Material Definition
	//----------------------------------------------------------------------------------

	// Material library name
	std::string mat_lib;

	// Material names for each group <grp number, mat name>
	std::map<u32, std::string> group_mat_names;

	// Vector of materials
	std::vector<Material> materials;
};


#include "obj_loader.tpp"
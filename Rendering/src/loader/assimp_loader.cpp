#include "assimp_loader.h"
#include "log/log.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "resource/resource_mgr.h"
#include "resource/model/material/material_factory.h"


void ProcessNodes(const aiNode* node, ModelOutput::Node& out) {

	out.name = node->mName.C_Str();

	for (u32 i = 0; i < node->mNumMeshes; ++i) {
		out.mesh_indices.push_back(node->mMeshes[i]);
	}

	for (u32 i = 0; i < node->mNumChildren; ++i) {
		ProcessNodes(node->mChildren[i], out.child_nodes.emplace_back());
	}
}


void ProcessMeshes(const aiScene* scene, ModelOutput& model_out) {

	for (u32 i = 0; i < scene->mNumMeshes; ++i) {

		const auto* mesh = scene->mMeshes[i];

		ModelOutput::MeshData out_mesh;
		out_mesh.name           = mesh->mName.C_Str();
		out_mesh.material_index = mesh->mMaterialIndex;

		//----------------------------------------------------------------------------------
		// Process index data
		//----------------------------------------------------------------------------------
		if (mesh->HasFaces()) {
			// The mesh should be triangulated when the model is imported
			for (u32 j = 0; j < mesh->mNumFaces; ++j) {
				out_mesh.indices.push_back(mesh->mFaces[j].mIndices[0]);
				out_mesh.indices.push_back(mesh->mFaces[j].mIndices[1]);
				out_mesh.indices.push_back(mesh->mFaces[j].mIndices[2]);
			}
		}

		//----------------------------------------------------------------------------------
		// Process vertex data
		//----------------------------------------------------------------------------------
		for (u32 j = 0; j < mesh->mNumVertices; ++j) {
			if (mesh->HasPositions()) {
				const auto& position = mesh->mVertices[j];
				out_mesh.positions.emplace_back(position.x, position.y, position.z);
			}

			if (mesh->HasNormals()) {
				const auto& normal = mesh->mNormals[j];
				out_mesh.normals.emplace_back(normal.x, normal.y, normal.z);
			}

			if (mesh->HasTangentsAndBitangents()) {
				const auto& tangent = mesh->mTangents[j];
				const auto& bitangent = mesh->mBitangents[j];
				out_mesh.tangents.emplace_back(tangent.x, tangent.y, tangent.z);
				out_mesh.bitangents.emplace_back(bitangent.x, bitangent.y, bitangent.z);
			}

			//TODO: support multiple texture coords per vertex
			if (mesh->HasTextureCoords(0)) {
				const auto& tex = mesh->mTextureCoords[0][j];
				out_mesh.texture_coords.emplace_back(tex.x, tex.y/*, tex.z*/);
			}

			//TODO: support vertex color sets
			if (mesh->HasVertexColors(0)) {
				const auto& color = mesh->mColors[0][j];
				out_mesh.colors.emplace_back(color.r, color.g, color.b);
			}
		}

		model_out.meshes.push_back(out_mesh);
	}
}


void ProcessMaterials(const aiScene* scene, fs::path base_path, ResourceMgr& resource_mgr, ModelOutput& model_out) {

	// Get a scalar value from a material
	static constexpr auto get_scalar = [](const aiMaterial* mat, const char* key, unsigned int type, unsigned int idx, auto& out) {
		mat->Get(key, type, idx, out);
	};

	// Get a color from a material
	static constexpr auto get_color = [](const aiMaterial* mat, const char* key, unsigned int type, unsigned int idx, auto& out) {
		aiColor3D color;
		if (mat->Get(key, type, idx, color) == aiReturn_SUCCESS) {
			out.x = color.r;
			out.y = color.g;
			out.z = color.b;
		}
	};

	// Get a texture from a material
	const auto get_map = [&](const aiMaterial* mat, aiTextureType type, ResourceMgr& resource_mgr, std::shared_ptr<Texture>& out) {
		aiString         path;
		aiTextureMapping mapping;
		unsigned int     uvindex;
		ai_real          blend;
		aiTextureOp      op;
		aiTextureMapMode mode;
		//TODO: handle texture stacks
		if (mat->GetTexture(type, /*index*/0, &path, &mapping, &uvindex, &blend, &op, &mode) == aiReturn_SUCCESS) {
			if (!path.data[0]) return;

			if (path.data[0] == '*') {
				//TODO: handle embedded textures? (very rare)
				Logger::log(LogLevel::info, "Embedded texture found in model");
			}
			else {
				out = resource_mgr.getOrCreate<Texture>(base_path / path.C_Str());
			}
		}
	};

	//----------------------------------------------------------------------------------
	// Process all materials in the scene
	//----------------------------------------------------------------------------------
	for (u32 i = 0; i < scene->mNumMaterials; ++i) {

		// Create output material struct
		Material out_mat = MaterialFactory::CreateDefaultMaterial(resource_mgr);

		// Get current material
		const auto* mat = scene->mMaterials[i];

		aiString name;
		if (mat->Get(AI_MATKEY_NAME, name) == aiReturn_SUCCESS) {
			out_mat.name = name.C_Str();
		}
		
		// Get colors
		get_color(mat, AI_MATKEY_COLOR_DIFFUSE,  out_mat.params.base_color);
		get_color(mat, AI_MATKEY_COLOR_EMISSIVE, out_mat.params.emissive);

		// Get maps
		get_map(mat, aiTextureType_DIFFUSE,  resource_mgr, out_mat.maps.base_color);
		get_map(mat, aiTextureType_HEIGHT,   resource_mgr, out_mat.maps.normal);  //Sometimes normal map will be stored in height maps section
		get_map(mat, aiTextureType_NORMALS,  resource_mgr, out_mat.maps.normal);
		get_map(mat, aiTextureType_EMISSIVE, resource_mgr, out_mat.maps.emissive);

		//TODO: get material parameter scalars and material parameter map

		model_out.materials.push_back(out_mat);
	}
}


ModelOutput AssimpLoad(ResourceMgr& resource_mgr,
                       const fs::path& file,
                       bool flip_winding,
                       bool flip_uv) {

	Assimp::Importer importer;

	// Set importer to remove lines and points from the model
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

	// Import flags
	u32 pflags = aiProcess_MakeLeftHanded        |    //Left handed coordinate system
	             aiProcess_GenUVCoords           |    //Convert texture mappings to UV
	             aiProcess_Triangulate           |    //Triangulate the mesh
	             aiProcess_GenNormals            |    //Generate normals if none exist
	             aiProcess_CalcTangentSpace      |    //Calculate vertex tangents
	             aiProcess_JoinIdenticalVertices |    //Join identical mesh vertices
	             //aiProcess_OptimizeMeshes        |    //Reduce the number of meshes
	             //aiProcess_OptimizeGraph         |    //Optimize the scene hierarchy
	             aiProcess_SortByPType;               //Split meshes by primitive type

	// Assimp uses opposite coordinates by default
	if (!flip_winding) pflags |= aiProcess_FlipWindingOrder;
	if (!flip_uv)      pflags |= aiProcess_FlipUVs;

	//----------------------------------------------------------------------------------
	// Load the model
	//----------------------------------------------------------------------------------
	const aiScene* scene = importer.ReadFile(file.string(), pflags);

	if (!scene) {
		Logger::log(LogLevel::err, "Error loading model: {}", file.string());
		return ModelOutput{};
	}

	//----------------------------------------------------------------------------------
	// Process the model
	//----------------------------------------------------------------------------------
	ModelOutput model_out;
	model_out.name = file.filename().string();
	model_out.file = file;

	ProcessNodes(scene->mRootNode, model_out.root);
	ProcessMeshes(scene, model_out);
	ProcessMaterials(scene, file.parent_path(), resource_mgr, model_out);

	return model_out;
}


namespace AssimpLoader {

ModelOutput Load(ResourceMgr& resource_mgr,
	                const fs::path& file,
	                bool flip_winding,
	                bool flip_uv) {

	return AssimpLoad(resource_mgr, file, flip_winding, flip_uv);
}

}

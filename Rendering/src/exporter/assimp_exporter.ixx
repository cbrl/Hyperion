module;

#include "io/io.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <assimp/Exporter.hpp>

#include "directx/d3d11.h"

export module rendering:exporter.assimp_exporter;

import :mesh;
import :model_blueprint;
import :texture;
import :exporter.texture_exporter;
import :vertex_types;


void ProcessNodes(aiNode& ai_node, const render::ModelBlueprint& bp, const render::ModelBlueprint::Node& bp_node) {

	//----------------------------------------------------------------------------------
	// Name
	//----------------------------------------------------------------------------------
	ai_node.mName.Set(bp_node.name);

	//----------------------------------------------------------------------------------
	// Mesh Indices
	//----------------------------------------------------------------------------------
	unsigned int* meshes = new unsigned int[bp_node.mesh_indices.size()];

	for (size_t i = 0; i < bp_node.mesh_indices.size(); ++i) {
		meshes[i] = bp_node.mesh_indices[i];
	}

	ai_node.mMeshes = meshes;
	ai_node.mNumMeshes = static_cast<unsigned int>(bp_node.mesh_indices.size());

	//----------------------------------------------------------------------------------
	// Children
	//----------------------------------------------------------------------------------
	ai_node.mNumChildren = static_cast<unsigned int>(bp_node.child_nodes.size());

	for (const auto& bp_child : bp_node.child_nodes) {
		aiNode* ai_child = new aiNode();
		ai_node.addChildren(1, &ai_child);
		ProcessNodes(*ai_child, bp, bp_child);
	}
}


template<typename VertexT>
void ReadMeshData(ID3D11Device& device,
                  ID3D11DeviceContext& device_context,
                  const render::Mesh& mesh,
                  std::vector<VertexT>& vertices,
                  std::vector<u32>& indices) {

	HRESULT hr;

	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;

	D3D11_BUFFER_DESC vb_desc = {};
	D3D11_BUFFER_DESC ib_desc = {};

	//----------------------------------------------------------------------------------
	// Vertex buffer
	//----------------------------------------------------------------------------------
	vb_desc.Usage = D3D11_USAGE_STAGING;
	vb_desc.ByteWidth = sizeof(VertexT) * mesh.getVertexCount();
	vb_desc.BindFlags = 0;
	vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;

	// Create vertex buffer
	hr = device.CreateBuffer(&vb_desc, NULL, vertex_buffer.GetAddressOf());
	if (FAILED(hr)) {
		Logger::log(LogLevel::err, "Failed to create staging vertex buffer for model export");
		return;
	}


	//----------------------------------------------------------------------------------
	// Index buffer
	//----------------------------------------------------------------------------------
	ib_desc.Usage = D3D11_USAGE_STAGING;
	ib_desc.ByteWidth = sizeof(u32) * mesh.getIndexCount();
	ib_desc.BindFlags = 0;
	ib_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	ib_desc.MiscFlags = 0;
	ib_desc.StructureByteStride = 0;

	// Create index buffer
	hr = device.CreateBuffer(&ib_desc, NULL, index_buffer.GetAddressOf());
	if (FAILED(hr)) {
		Logger::log(LogLevel::err, "Failed to create staging index buffer for model export");
		return;
	}


	//----------------------------------------------------------------------------------
	// Copy the mesh data into the buffers
	//----------------------------------------------------------------------------------
	device_context.CopyResource(vertex_buffer.Get(), mesh.getVertexBuffer());
	device_context.CopyResource(index_buffer.Get(), mesh.getIndexBuffer());

	D3D11_MAPPED_SUBRESOURCE mapped_resource;

	// Copy vertices
	hr = device_context.Map(vertex_buffer.Get(), NULL, D3D11_MAP_READ, NULL, &mapped_resource);
	if (SUCCEEDED(hr)) {
		const auto* buffer_data = static_cast<VertexT*>(mapped_resource.pData);
		for (size_t i = 0; i < mesh.getVertexCount(); ++i) {
			vertices.push_back(buffer_data[i]);
		}
		device_context.Unmap(vertex_buffer.Get(), NULL);
	}
	else {
		Logger::log(LogLevel::err, "Failed to map staging vertex buffer for model export");
		return;
	}

	// Copy indices
	hr = device_context.Map(index_buffer.Get(), NULL, D3D11_MAP_READ, NULL, &mapped_resource);
	if (SUCCEEDED(hr)) {
		const auto* buffer_data = static_cast<u32*>(mapped_resource.pData);
		for (size_t i = 0; i < mesh.getVertexCount(); ++i) {
			indices.push_back(buffer_data[i]);
		}
		device_context.Unmap(index_buffer.Get(), NULL);
	}
	else {
		vertices.clear();
		Logger::log(LogLevel::err, "Failed to map staging vertex buffer for model export");
		return;
	}
}


template<typename VertexT>
void ProcessMesh(ID3D11Device& device,
                 ID3D11DeviceContext& device_context,
                 aiMesh& ai_mesh,
                 const render::Mesh& bp_mesh,
                 u32 mat_index) {

	ai_mesh.mName = bp_mesh.getName();
	ai_mesh.mMaterialIndex = mat_index;

	std::vector<VertexT> vertices;
	std::vector<u32> indices;

	//----------------------------------------------------------------------------------
	// Read data from mesh buffers
	//----------------------------------------------------------------------------------
	ReadMeshData<VertexT>(device, device_context, bp_mesh, vertices, indices);
	if (vertices.empty()) {
		return;
	}

	//----------------------------------------------------------------------------------
	// Create faces
	//----------------------------------------------------------------------------------
	ai_mesh.mFaces = new aiFace[vertices.size() / 3];
	ai_mesh.mNumFaces = static_cast<unsigned int>(vertices.size() / 3);

	for (size_t i = 0; i < (indices.size() / 3); ++i) {
		ai_mesh.mFaces[i].mIndices = new unsigned int[3];
		ai_mesh.mFaces[i].mNumIndices = 3;

		ai_mesh.mFaces[i].mIndices[0] = indices[i * 3];
		ai_mesh.mFaces[i].mIndices[1] = indices[i * 3 + 1];
		ai_mesh.mFaces[i].mIndices[2] = indices[i * 3 + 2];
	}

	//----------------------------------------------------------------------------------
	// Create Assimp compatible data arrays and assign the mesh fields
	//----------------------------------------------------------------------------------
	ai_mesh.mVertices = new aiVector3D[vertices.size()];
	ai_mesh.mNumVertices = static_cast<unsigned int>(vertices.size());

	if constexpr (VertexT::hasColor()) {
		ai_mesh.mColors[0] = new aiColor4D[vertices.size()];
	}
	if constexpr (VertexT::hasNormal()) {
		ai_mesh.mNormals = new aiVector3D[vertices.size()];
	}
	if constexpr (VertexT::hasTexture()) {
		ai_mesh.mTextureCoords[0] = new aiVector3D[vertices.size()];
		std::fill(ai_mesh.mNumUVComponents, ai_mesh.mNumUVComponents + 8, 2); //2D texture coords
	}

	for (size_t i = 0; i < vertices.size(); ++i) {
		ai_mesh.mVertices[i].x = vertices[i].position[0];
		ai_mesh.mVertices[i].y = vertices[i].position[1];
		ai_mesh.mVertices[i].z = vertices[i].position[2];

		if constexpr (VertexT::hasColor()) {
			ai_mesh.mColors[0][i].r = vertices[i].color[0];
			ai_mesh.mColors[0][i].g = vertices[i].color[1];
			ai_mesh.mColors[0][i].b = vertices[i].color[2];
			ai_mesh.mColors[0][i].a = vertices[i].color[3];
		}

		if constexpr (VertexT::hasNormal()) {
			ai_mesh.mNormals[i].x = vertices[i].normal[0];
			ai_mesh.mNormals[i].y = vertices[i].normal[1];
			ai_mesh.mNormals[i].z = vertices[i].normal[2];
		}

		if constexpr (VertexT::hasTexture()) {
			ai_mesh.mTextureCoords[0][i].x = vertices[i].texCoord[0];
			ai_mesh.mTextureCoords[0][i].y = vertices[i].texCoord[1];
		}
	}
}


void ProcessMeshes(ID3D11Device& device,
                   ID3D11DeviceContext& device_context,
                   aiScene& scene,
                   const render::ModelBlueprint& bp) {

	scene.mMeshes = new aiMesh* [bp.meshes.size()] { nullptr };
	scene.mNumMeshes = static_cast<unsigned int>(bp.meshes.size());

	for (size_t i = 0; i < bp.meshes.size(); ++i) {
		scene.mMeshes[i] = new aiMesh();
		scene.mMeshes[i]->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;

		const std::type_index& vertex_type = bp.meshes[i].getVertexType();

		// Hacky method of getting correct vertex data
		if (vertex_type == typeid(VertexPosition)) {
			ProcessMesh<VertexPosition>(device, device_context, *scene.mMeshes[i], bp.meshes[i], bp.mat_indices[i]);
		}
		if (vertex_type == typeid(VertexPositionColor)) {
			ProcessMesh<VertexPositionColor>(device, device_context, *scene.mMeshes[i], bp.meshes[i], bp.mat_indices[i]);
		}
		if (vertex_type == typeid(VertexPositionTexture)) {
			ProcessMesh<VertexPositionTexture>(device, device_context, *scene.mMeshes[i], bp.meshes[i], bp.mat_indices[i]);
		}
		if (vertex_type == typeid(VertexPositionNormal)) {
			ProcessMesh<VertexPositionNormal>(device, device_context, *scene.mMeshes[i], bp.meshes[i], bp.mat_indices[i]);
		}
		if (vertex_type == typeid(VertexPositionNormalColor)) {
			ProcessMesh<VertexPositionNormalColor>(device, device_context, *scene.mMeshes[i], bp.meshes[i], bp.mat_indices[i]);
		}
		if (vertex_type == typeid(VertexPositionNormalTexture)) {
			ProcessMesh<VertexPositionNormalTexture>(device, device_context, *scene.mMeshes[i], bp.meshes[i], bp.mat_indices[i]);
		}
	}
}


// Set a scalar value in a material
template<typename T>
bool SetScalar(aiMaterial& mat, const char* key, unsigned int type, unsigned int idx, T& scalar) {
	return mat.AddProperty(&scalar, 1, key, type, idx) == aiReturn_SUCCESS;
};


// Set a color in a material
template<typename T>
bool SetColor(aiMaterial& mat, const char* key, unsigned int type, unsigned int idx, T& color) {
	aiColor3D ai_color;
	ai_color.r = color[0];
	ai_color.g = color[1];
	ai_color.b = color[2];
	return mat.AddProperty(&ai_color, 1, key, type, idx) == aiReturn_SUCCESS;
};


// Set a texture in a material and export the texture
bool ExportMap(aiMaterial& mat,
               const char* key,
               unsigned int type,
               unsigned int idx,
               const render::Texture& texture,
               const fs::path& parent_path,
               ID3D11Device& device,
               ID3D11DeviceContext& device_context) {

	const fs::path guid = texture.getGUID();
	const fs::path file = parent_path / guid.filename().replace_extension("tga");

	aiString ai_file;
	ai_file.Set(file.string());

	if (mat.AddProperty(&ai_file, key, type, idx) == aiReturn_SUCCESS) {
		return render::exporter::ExportTextureToTGA(device, device_context, texture, file);
	}
	return false;
};

void ProcessMaterials(ID3D11Device& device,
                      ID3D11DeviceContext& device_context,
                      aiScene& scene,
                      const render::ModelBlueprint& bp,
                      const fs::path& parent_path) {

	scene.mMaterials    = new aiMaterial*[bp.materials.size()]{ nullptr };
	scene.mNumMaterials = static_cast<unsigned int>(bp.materials.size());

	for (size_t i = 0; i < bp.materials.size(); ++i) {
		scene.mMaterials[i] = new aiMaterial();

		aiMaterial& ai_mat = *scene.mMaterials[i];
		const render::Material& bp_mat = bp.materials[i];

		//----------------------------------------------------------------------------------
		// Set material name
		//----------------------------------------------------------------------------------
		aiString name;
		name.Set(bp_mat.name);
		ai_mat.AddProperty(&name, AI_MATKEY_NAME);

		//----------------------------------------------------------------------------------
		// Set material parameters
		//----------------------------------------------------------------------------------
		SetColor(ai_mat,  AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, bp_mat.params.base_color);
		SetScalar(ai_mat, AI_MATKEY_OPACITY,                                     bp_mat.params.base_color[3]);
		SetColor(ai_mat,  AI_MATKEY_COLOR_EMISSIVE,                              bp_mat.params.emissive);
		SetScalar(ai_mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR,   bp_mat.params.metalness);
		SetScalar(ai_mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR,  bp_mat.params.roughness);

		//----------------------------------------------------------------------------------
		// Export texture maps
		//----------------------------------------------------------------------------------
		using namespace std::placeholders;
		
		// Bind arguments for brevity
		auto export_func = std::bind(ExportMap, std::ref(ai_mat), _1, _2, _3, _4, parent_path, std::ref(device), std::ref(device_context));

		if (bp_mat.maps.base_color) {
			export_func(AI_MATKEY_TEXTURE(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, *bp_mat.maps.base_color));
		}
		if (bp_mat.maps.normal) {
			export_func(AI_MATKEY_TEXTURE_NORMALS(0), *bp_mat.maps.normal);
		}
		if (bp_mat.maps.material_params) {
			export_func(AI_MATKEY_TEXTURE(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, *bp_mat.maps.material_params));
		}
		if (bp_mat.maps.emissive) {
			export_func(AI_MATKEY_TEXTURE_EMISSIVE(0), *bp_mat.maps.emissive);
		}
	}
}


export namespace render::exporter::detail {

void AssimpExport(ID3D11Device& device, ID3D11DeviceContext& device_context, const ModelBlueprint& blueprint, fs::path filename) {
	// The scene's destructor will delete everything created here with operator new()
	aiScene scene;
	scene.mRootNode = new aiNode();

	// Build the scene
	ProcessNodes(*scene.mRootNode, blueprint, blueprint.root);
	ProcessMeshes(device, device_context, scene, blueprint);
	ProcessMaterials(device, device_context, scene, blueprint, filename.parent_path());

	// Export the scene in glTF 2 format
	Assimp::Exporter exporter;
	const unsigned int flags = aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs;
	exporter.Export(&scene, "gltf2", filename.replace_extension("gltf").string(), flags);
}

} //namespace render::exporter::detail

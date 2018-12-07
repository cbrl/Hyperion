
namespace BlueprintFactory {

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateCube(ResourceMgr& resource_mgr,
	                                      const ModelConfig<VertexT>& config,
	                                      f32 size,
	                                      bool invertn) {

		// Model name
		std::string name = "Cube " + std::to_string(size) + (config.flip_winding ? " RH" : "") + (invertn ? " InvertN" : "");

		
		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeCube(vertices, mesh.indices, size, config.flip_winding, invertn);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Cube Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateBox(ResourceMgr& resource_mgr,
	                                     const ModelConfig<VertexT>& config,
	                                     const vec3_f32& size,
	                                     bool invertn) {

		// Model name
		std::string name = "Box " + std::to_string(size.x) + std::to_string(size.y) + std::to_string(size.z) + (config.flip_winding ? " RH" : "") + (
			              invertn ? " InvertN" : "");


		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeBox(vertices, mesh.indices, size, config.flip_winding, invertn);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Box Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateSphere(ResourceMgr& resource_mgr,
	                                        const ModelConfig<VertexT>& config,
	                                        f32 diameter,
	                                        size_t tessellation,
	                                        bool invertn) {

		// Model name
		std::string name = "Sphere " + std::to_string(diameter) + std::to_string(tessellation) + (config.flip_winding ? " RH" : "") + (
			              invertn ? " InvertN" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeSphere(vertices, mesh.indices, diameter, tessellation, config.flip_winding, invertn);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Sphere Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateGeoSphere(ResourceMgr& resource_mgr,
	                                           const ModelConfig<VertexT>& config,
	                                           f32 diameter,
	                                           size_t tessellation) {

		// Model name
		std::string name = "GeoSphere " + std::to_string(diameter) + std::to_string(tessellation) + (config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeGeoSphere(vertices, mesh.indices, diameter, tessellation, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "GeoSphere Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateCylinder(ResourceMgr& resource_mgr,
	                                          const ModelConfig<VertexT>& config,
	                                          f32 diameter,
	                                          f32 height,
	                                          size_t tessellation) {

		// Model name
		std::string name = "Cylinder " + std::to_string(height) + std::to_string(diameter) + std::to_string(tessellation) + (
			              config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeCylinder(vertices, mesh.indices, diameter, height, tessellation, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Cylinder Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateCone(ResourceMgr& resource_mgr,
	                                      const ModelConfig<VertexT>& config,
	                                      f32 diameter,
	                                      f32 height,
	                                      size_t tessellation) {

		// Model name
		std::string name = "Cone " + std::to_string(diameter) + std::to_string(height) + std::to_string(tessellation) + (
			              config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeCone(vertices, mesh.indices, diameter, height, tessellation, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Cone Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateTorus(ResourceMgr& resource_mgr,
	                                       const ModelConfig<VertexT>& config,
	                                       f32 diameter,
	                                       f32 thickness,
	                                       size_t tessellation) {

		// Model name
		std::string name = "Torus " + std::to_string(diameter) + std::to_string(thickness) + std::to_string(tessellation) + (
			              config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeTorus(vertices, mesh.indices, diameter, thickness, tessellation, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Torus Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateTetrahedron(ResourceMgr& resource_mgr,
	                                             const ModelConfig<VertexT>& config,
	                                             f32 size) {

		// Model name
		std::string name = "Tetrahedron " + std::to_string(size) + (config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeTetrahedron(vertices, mesh.indices, size, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Tetrahedron Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateOctahedron(ResourceMgr& resource_mgr,
	                                            const ModelConfig<VertexT>& config,
	                                            f32 size) {

		// Model name
		std::string name = "Octahedron " + std::to_string(size) + (config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeOctahedron(vertices, mesh.indices, size, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Octahedron Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateDodecahedron(ResourceMgr& resource_mgr,
	                                              const ModelConfig<VertexT>& config,
	                                              f32 size) {

		// Model name
		std::string name = "Dodecahedron " + std::to_string(size) + (config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeDodecahedron(vertices, mesh.indices, size, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Dodecahedron Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}


	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateIcosahedron(ResourceMgr& resource_mgr,
	                                             const ModelConfig<VertexT>& config,
	                                             f32 size) {

		// Model name
		std::string name = "Icosahedron " + std::to_string(size) + (config.flip_winding ? " RH" : "");

		// Create the mesh
		ModelOutput::MeshData mesh;

		std::vector<VertexT> vertices;
		Shapes::ComputeIcosahedron(vertices, mesh.indices, size, config.flip_winding);

		for (const auto& v : vertices) {
			mesh.positions.push_back(v.position);
			if constexpr (VertexT::hasNormal()) mesh.normals.push_back(v.normal);
			if constexpr (VertexT::hasTexture()) mesh.texture_coords.push_back(v.texCoord);
		}


		// Create the material
		Material mat;
		mat.name                 = "Icosahedron Material";
		mat.params.ambient       = vec4_f32{0.1f, 0.1f, 0.1f};
		mat.params.diffuse       = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.specular      = vec4_f32{1.0f, 1.0f, 1.0f};
		mat.params.spec_scale    = 1.0f;
		mat.params.spec_exponent = 20.0f;


		// Create the ModelOutput object
		ModelOutput out;
		out.name = name;
		out.root.name = name;
		out.root.mesh_indices.push_back(0);
		out.materials.push_back(std::move(mat));
		out.meshes.push_back(std::move(mesh));


		return resource_mgr.getOrCreate<ModelBlueprint>(StrToWstr(name), out, config);
	}
}

namespace BlueprintFactory {

	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateCube(ResourceMgr& resource_mgr,
	                                      f32 size,
	                                      bool rhcoords,
	                                      bool invertn) {

		// Model name
		std::string name = "Cube " + std::to_string(size) + (rhcoords ? " RH" : "") + (invertn ? " InvertN" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeCube(vertices, indices, size, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name        = "Cube Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Cube";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateBox(ResourceMgr& resource_mgr,
	                                     const vec3_f32& size,
	                                     bool rhcoords,
	                                     bool invertn) {

		// Model name
		std::string name = "Box " + std::to_string(size.x) + std::to_string(size.y) + std::to_string(size.z) + (rhcoords ? " RH" : "") + (
			              invertn ? " InvertN" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeBox(vertices, indices, size, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name        = "Box Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Box";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateSphere(ResourceMgr& resource_mgr,
	                                        f32 diameter,
	                                        size_t tessellation,
	                                        bool rhcoords,
	                                        bool invertn) {

		// Model name
		std::string name = "Sphere " + std::to_string(diameter) + std::to_string(tessellation) + (rhcoords ? " RH" : "") + (
			              invertn ? " InvertN" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeSphere(vertices, indices, diameter, tessellation, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name        = "Sphere Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Sphere";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateGeoSphere(ResourceMgr& resource_mgr,
	                                           f32 diameter,
	                                           size_t tessellation,
	                                           bool rhcoords) {

		// Model name
		std::string name = "GeoSphere " + std::to_string(diameter) + std::to_string(tessellation) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeGeoSphere(vertices, indices, diameter, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "GeoSphere Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "GeoSphere";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateCylinder(ResourceMgr& resource_mgr,
	                                          f32 diameter,
	                                          f32 height,
	                                          size_t tessellation,
	                                          bool rhcoords) {

		// Model name
		std::string name = "Cylinder " + std::to_string(height) + std::to_string(diameter) + std::to_string(tessellation) + (
			              rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeCylinder(vertices, indices, diameter, height, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Cylinder Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Cylinder";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateCone(ResourceMgr& resource_mgr,
	                                      f32 diameter,
	                                      f32 height,
	                                      size_t tessellation,
	                                      bool rhcoords) {

		// Model name
		std::string name = "Cone " + std::to_string(diameter) + std::to_string(height) + std::to_string(tessellation) + (
			              rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeCone(vertices, indices, diameter, height, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Cone Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Cone";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateTorus(ResourceMgr& resource_mgr,
	                                       f32 diameter,
	                                       f32 thickness,
	                                       size_t tessellation,
	                                       bool rhcoords) {

		// Model name
		std::string name = "Torus " + std::to_string(diameter) + std::to_string(thickness) + std::to_string(tessellation) + (
			              rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeTorus(vertices, indices, diameter, thickness, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Torus Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Torus";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateTetrahedron(ResourceMgr& resource_mgr,
	                                             f32 size,
	                                             bool rhcoords) {

		// Model name
		std::string name = "Tetrahedron " + std::to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeTetrahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Tetrahedron Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Tetrahedron";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateOctahedron(ResourceMgr& resource_mgr,
	                                            f32 size,
	                                            bool rhcoords) {

		// Model name
		std::string name = "Octahedron " + std::to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeOctahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Octahedron Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Octahedron";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateDodecahedron(ResourceMgr& resource_mgr,
	                                              f32 size,
	                                              bool rhcoords) {

		// Model name
		std::string name = "Dodecahedron " + std::to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeDodecahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Dodecahedron Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Dodecahedron";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}


	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateIcosahedron(ResourceMgr& resource_mgr,
	                                             f32 size,
	                                             bool rhcoords) {

		// Model name
		std::string name = "Icosahedron " + std::to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeIcosahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Icosahedron Material";
		mat.ambient     = vec4_f32(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = vec4_f32(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = vec4_f32(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		vector<Material> materials;
		materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start    = 0;
		grp.material_index = 0;
		grp.name           = "Icosahedron";

		vector<Group> groups;
		groups.push_back(grp);


		// Create the ModelOutput object
		ModelOutput<VertexT> out(name, vertices, indices, materials, groups);


		return resource_mgr.getOrCreate<ModelBlueprint>(str2wstr(name), out);
	}
}

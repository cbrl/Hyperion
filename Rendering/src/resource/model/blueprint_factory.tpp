namespace BlueprintFactory {

	template<typename VertexT>
	[[nodiscard]]
	shared_ptr<ModelBlueprint> CreateCube(ResourceMgr& resource_mgr,
	                                      f32 size,
	                                      bool rhcoords,
	                                      bool invertn) {

		// Model name
		string name = "Cube " + to_string(size) + (rhcoords ? " RH" : "") + (invertn ? " InvertN" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeCube(vertices, indices, size, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name        = "Cube Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
	                                     const f32_3& size,
	                                     bool rhcoords,
	                                     bool invertn) {

		// Model name
		string name = "Box " + to_string(size.x) + to_string(size.y) + to_string(size.z) + (rhcoords ? " RH" : "") + (
			              invertn ? " InvertN" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeBox(vertices, indices, size, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name        = "Box Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Sphere " + to_string(diameter) + to_string(tessellation) + (rhcoords ? " RH" : "") + (
			              invertn ? " InvertN" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeSphere(vertices, indices, diameter, tessellation, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name        = "Sphere Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "GeoSphere " + to_string(diameter) + to_string(tessellation) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeGeoSphere(vertices, indices, diameter, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "GeoSphere Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Cylinder " + to_string(height) + to_string(diameter) + to_string(tessellation) + (
			              rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeCylinder(vertices, indices, diameter, height, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Cylinder Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Cone " + to_string(diameter) + to_string(height) + to_string(tessellation) + (
			              rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeCone(vertices, indices, diameter, height, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Cone Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Torus " + to_string(diameter) + to_string(thickness) + to_string(tessellation) + (
			              rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeTorus(vertices, indices, diameter, thickness, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Torus Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Tetrahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeTetrahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Tetrahedron Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Octahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeOctahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Octahedron Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Dodecahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeDodecahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Dodecahedron Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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
		string name = "Icosahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		vector<VertexT> vertices;
		vector<u32> indices;

		Shapes::ComputeIcosahedron(vertices, indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name        = "Icosahedron Material";
		mat.ambient     = f32_4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.diffuse     = f32_4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.specular    = f32_4(1.0f, 1.0f, 1.0f, 20.0f);
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

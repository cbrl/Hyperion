
namespace BlueprintFactory {
	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateCube(ID3D11Device* device, float size, bool rhcoords, bool invertn) {

		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Cube " + to_string(size) + (rhcoords ? " RH" : "") + (invertn ? " InvertN" : "");


		// Compute the vertices and indices
		Shapes::ComputeCube(out.vertices, out.indices, size, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name = "Cube Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Cube";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateBox(ID3D11Device* device, const float3& size, bool rhcoords, bool invertn) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Box " + to_string(size.x) + to_string(size.y) + to_string(size.z) + (rhcoords ? " RH" : "") + (invertn ? " InvertN" : "");


		// Compute the vertices and indices
		Shapes::ComputeBox(out.vertices, out.indices, size, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name = "Box Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Box";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateSphere(ID3D11Device* device, float diameter, size_t tessellation, bool rhcoords, bool invertn) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Sphere " + to_string(diameter) + (rhcoords ? " RH" : "") + (invertn ? " InvertN" : "");


		// Compute the vertices and indices
		Shapes::ComputeSphere(out.vertices, out.indices, diameter, tessellation, rhcoords, invertn);


		// Create the material
		Material mat;
		mat.name = "Sphere Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Sphere";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateGeoSphere(ID3D11Device* device, float diameter, size_t tessellation, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "GeoSphere " + to_string(diameter) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeGeoSphere(out.vertices, out.indices, diameter, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name = "GeoSphere Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "GeoSphere";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateCylinder(ID3D11Device* device, float diameter, float height, size_t tessellation, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Cylinder " + to_string(height) + to_string(diameter) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeCylinder(out.vertices, out.indices, diameter, height, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Cylinder Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Cylinder";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateCone(ID3D11Device* device, float diameter, float height, size_t tessellation, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Cone " + to_string(diameter) + to_string(height) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeCone(out.vertices, out.indices, diameter, height, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Cone Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Cone";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateTorus(ID3D11Device* device, float diameter, float thickness, size_t tessellation, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Torus " + to_string(diameter) + to_string(thickness) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeTorus(out.vertices, out.indices, diameter, thickness, tessellation, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Torus Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Torus";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateTetrahedron(ID3D11Device* device, float size, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Tetrahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeTetrahedron(out.vertices, out.indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Tetrahedron Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Tetrahedron";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateOctahedron(ID3D11Device* device, float size, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Octahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeOctahedron(out.vertices, out.indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Octahedron Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Octahedron";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateDodecahedron(ID3D11Device* device, float size, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Dodecahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeDodecahedron(out.vertices, out.indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Dodecahedron Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Dodecahedron";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}

	template<typename VertexT>
	shared_ptr<ModelBlueprint> CreateIcosahedron(ID3D11Device * device, float size, bool rhcoords) {
		// Create the ModelOutput object
		ModelOutput<VertexT> out;
		out.name = "Icosahedron " + to_string(size) + (rhcoords ? " RH" : "");


		// Compute the vertices and indices
		Shapes::ComputeIcosahedron(out.vertices, out.indices, size, rhcoords);


		// Create the material
		Material mat;
		mat.name = "Icosahedron Material";
		mat.Ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Kd = float4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ks = float4(1.0f, 1.0f, 1.0f, 20.0f);
		mat.has_texture = false;

		out.materials.push_back(mat);


		// Create the group definition
		Group grp;
		grp.index_start = 0;
		grp.material_index = 0;
		grp.name = "Icosahedron";

		out.groups.push_back(grp);


		return make_shared<ModelBlueprint>(device, out);
	}
}
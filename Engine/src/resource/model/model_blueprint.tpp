template<typename VertexT>
void ModelBlueprint::constructBlueprint(ID3D11Device& device, const ModelOutput<VertexT>& out) {

	// Copy members
	name        = out.name;
	materials   = out.materials;
	aabb        = out.aabb;
	sphere      = out.sphere;
	model_parts = out.model_parts;


	// Create the mesh
	mesh = make_shared<Mesh>(device, out.vertices, out.indices);
}

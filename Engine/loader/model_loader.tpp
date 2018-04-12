
namespace ModelLoader {

	template<typename VertexT>
	static ModelOutput<VertexT> Load(ID3D11Device* device,
									 ResourceMgr& resource_mgr,
									 const wstring& filename) {

		// TODO:
		// Check file extension, then use the appropriate loader. For
		// now, the only loader is the obj loader.

		// Create a model blueprint from an obj file
		ModelOutput<VertexT> out = OBJLoader<VertexT>::Load(device, resource_mgr, filename, false);
		FILE_LOG(logDEBUG) << "Loaded OBJ file: " << wstr2str(filename);

		return out;
	}
}
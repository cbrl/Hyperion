namespace ModelLoader {

	template<typename VertexT>
	static ModelOutput<VertexT> load(ResourceMgr& resource_mgr, const wstring& filename) {

		// TODO:
		// Check file extension, then use the appropriate loader. For
		// now, the only loader is the obj loader.

		// Create a model blueprint from an obj file
		ModelOutput<VertexT> out = ObjLoader<VertexT>::load(resource_mgr, filename, false);
		FILE_LOG(logDEBUG) << "Loaded OBJ file: " << wstr2str(filename);

		return out;
	}
}

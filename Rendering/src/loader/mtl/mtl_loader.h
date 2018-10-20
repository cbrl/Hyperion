#pragma once

#include "io/io.h"
#include "io/line_reader.h"
#include "resource/model/material/material.h"

class ResourceMgr;

class MtlLoader final : public LineReader {
public:
	MtlLoader(ResourceMgr& resource_mgr, std::vector<Material>& materials);
	MtlLoader(const MtlLoader& loader) = delete;
	MtlLoader(MtlLoader&& loader) = delete;

	~MtlLoader() = default;

	MtlLoader& operator=(const MtlLoader& loader) = delete;
	MtlLoader& operator=(MtlLoader&& loader) = delete;


	void load(const fs::path& file);


private:
	void readLine() override;


private:
	ResourceMgr& resource_mgr;
	std::vector<Material>& materials;
};
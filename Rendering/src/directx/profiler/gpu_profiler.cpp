#include "gpu_profiler.h"


GPUProfiler::GPUProfiler(ID3D11Device& device, ID3D11DeviceContext& device_context)
    : device(device)
    , device_context(device_context)
    , query_frame(0)
    , read_frame(-1) {

	static constexpr D3D11_QUERY_DESC disjoint_desc = {D3D11_QUERY_TIMESTAMP_DISJOINT, 0};
	for (auto& ptr : timestamp_disjoint) {
		HRESULT hr = device.CreateQuery(&disjoint_desc, ptr.GetAddressOf());
	}

	registerTimestampID(GPUTimestamps::frame);
	registerTimestampID(GPUTimestamps::render_scene);
	registerTimestampID(GPUTimestamps::shadow_maps);
	registerTimestampID(GPUTimestamps::forward_render);
	registerTimestampID(GPUTimestamps::deferred_render);
	registerTimestampID(GPUTimestamps::render_opaque);
	registerTimestampID(GPUTimestamps::render_transparent);
}


void GPUProfiler::registerTimestampID(const std::string& identifier) {
	static constexpr D3D11_QUERY_DESC timestamp_desc = {D3D11_QUERY_TIMESTAMP, 0};

	// Create the begin/end queries for the stage
	HRESULT hr1, hr2;
	for (u8 i = 0; i < buffer_size; ++i) {
		auto result = timestamps[i].try_emplace(identifier); //result = pair<iterator, bool>
		auto& query_pair = result.first->second;
		hr1 = device.CreateQuery(&timestamp_desc, query_pair.first.GetAddressOf());
		hr2 = device.CreateQuery(&timestamp_desc, query_pair.second.GetAddressOf());
	}

	if (FAILED(hr1) || FAILED(hr2)) {
		Logger::log(LogLevel::err, "Failed to register timestamp ID: {}", identifier);
	} else {
		timestamp_ids.push_back(identifier); //add the ID to the list of registered IDs
	}
}


const std::vector<std::string>& GPUProfiler::getTimestampIDs() const noexcept {
	return timestamp_ids;
}


void GPUProfiler::beginFrame() {
	device_context.Begin(timestamp_disjoint[query_frame].Get());
	beginTimestamp(GPUTimestamps::frame);
}


void GPUProfiler::endFrame() {
	endTimestamp(GPUTimestamps::frame);
	device_context.End(timestamp_disjoint[query_frame].Get());
	query_frame += 1;
	if (query_frame >= buffer_size) query_frame = 0;
}


void GPUProfiler::beginTimestamp(const std::string& identifier) {
	if (auto it = timestamps[query_frame].find(identifier); it != timestamps[query_frame].end()) {
		device_context.End(it->second.first.Get());
	} else {
		Logger::log(LogLevel::warn, "Unknown timestamp identifier: \"{}\"", identifier);
	}
}


void GPUProfiler::endTimestamp(const std::string& identifier) {
	if (auto it = timestamps[query_frame].find(identifier); it != timestamps[query_frame].end()) {
		device_context.End(it->second.second.Get());
	} else {
		Logger::log(LogLevel::warn, "Unknown timestamp identifier: \"{}\"", identifier);
	}
}


void GPUProfiler::update() {

	timer.tick();

	// Skip frame if the profiler hasn't run long enough (read_frame is set to -1 on init)
	if (read_frame < 0) {
		if (query_frame < (buffer_size - 1))
			read_frame = 0;
		return;
	}

	// Store the current frame to read and update read_frame
	const auto frame = static_cast<u8>(read_frame);
	if (read_frame >= buffer_size - 1)
		read_frame = 0;
	else
		read_frame += 1;

	/* Wait for data to be ready?
	while (device_context.GetData(timestamp_disjoint[frame].Get(), nullptr, 0, 0) == S_FALSE) {
		Sleep(1);
	}
	*/

	// Get the disjoint timestamp data
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint_ts;
	if (device_context.GetData(timestamp_disjoint[frame].Get(), &disjoint_ts, sizeof(disjoint_ts), 0) != S_OK) {
		Logger::log(LogLevel::debug, "Failed to read disjoint timestamp data");
		return;
	}

	// Skip this frame if the timestamp is disjointed
	if (disjoint_ts.Disjoint) {
		Logger::log(LogLevel::debug, "Discarding disjointed timestamp");
		return;
	}

	// Calculate the elapsed time for each stage
	for (auto& id : timestamp_ids) {
		UINT64 begin_ts = 0;
		UINT64 end_ts = 0;

		// Get the stage begin time
		if (device_context.GetData(timestamps[frame][id].first.Get(), &begin_ts, sizeof(begin_ts), 0) != S_OK) {
			//Logger::log(LogLevel::debug, "Failed to read \"{}\" begin timestamp data", id);
		}

		// Get the stage end time
		if (device_context.GetData(timestamps[frame][id].second.Get(), &end_ts, sizeof(end_ts), 0) != S_OK) {
			//Logger::log(LogLevel::debug, "Failed to read \"{}\" end timestamp data", id);
		}

		f32 time = static_cast<f32>(end_ts - begin_ts) / static_cast<f32>(disjoint_ts.Frequency);
		stage_times[id] = time;
		total_stage_times[id] += time;
	}

	// Update the elapsed time and frame count, and update the avg times if enough time has passed.
	++frame_count;
	if (timer.totalTime() >= 500ms) {
		for (auto& id : timestamp_ids) {
			avg_stage_times[id] = total_stage_times[id] / static_cast<f32>(frame_count);
			total_stage_times[id] = 0.0f;
		}
		frame_count = 0;
		timer.reset();
	}
}


f32 GPUProfiler::deltaTime(const std::string& id) const {
	if (auto it = stage_times.find(id); it != stage_times.end()) {
		return it->second;
	}
	return 0.0f;
}


f32 GPUProfiler::averageTime(const std::string& id) const {
	if (auto it = avg_stage_times.find(id); it != avg_stage_times.end()) {
		return it->second;
	}
	return 0.0f;
}
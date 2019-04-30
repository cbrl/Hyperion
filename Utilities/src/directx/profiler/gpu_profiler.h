#pragma once

#include "datatypes/datatypes.h"
#include "directx/d3d11.h"
#include "time/stopwatch.h"
#include "log/log.h"


struct GPUTimestamps {
	static constexpr gsl::czstring<> frame              = "Frame";
	static constexpr gsl::czstring<> render_scene       = "Render Scene";
	static constexpr gsl::czstring<> shadow_maps        = "Shadow Maps";
	static constexpr gsl::czstring<> forward_render     = "Forward Render";
	static constexpr gsl::czstring<> forwardplus_render = "Forward+ Render";
	static constexpr gsl::czstring<> deferred_render    = "Deferred Render";
	static constexpr gsl::czstring<> render_opaque      = "Render Opaque";
	static constexpr gsl::czstring<> render_transparent = "Render Transparent";
};


class GPUProfiler {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	GPUProfiler(ID3D11Device& device, ID3D11DeviceContext& device_context);
	GPUProfiler(const GPUProfiler&) = delete;
	GPUProfiler(GPUProfiler&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~GPUProfiler() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	GPUProfiler& operator=(const GPUProfiler&) = delete;
	GPUProfiler& operator=(GPUProfiler&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Register a new timestamp ID
	void registerTimestampID(const std::string& identifier);

	// Get the list of timestamp IDs
	const std::vector<std::string>& getTimestampIDs() const noexcept;

	// Begin profiling a new GPU frame
	void beginFrame();

	// End the current GPU frame
	void endFrame();

	// Mark the beginning of the stage
	void beginTimestamp(const std::string& identifier);

	// Mark the end of the stage
	void endTimestamp(const std::string& identifier);

	// Get the data from the earliest frame
	void update();

	// Get the last frame's time to render the stage represented by the provided ID
	[[nodiscard]]
	f32 deltaTime(const std::string& id) const;

	// Get the average time to render the stage represented by the provided ID
	[[nodiscard]]
	f32 averageTime(const std::string& id) const;


private:
	// Number of frames to buffer before reading data. Must be 2 or more.
	static constexpr u8 buffer_size = 8;

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Reference to the D3D device and device context
	ID3D11Device&        device;
	ID3D11DeviceContext& device_context;

	// A timer used for averaging the stage times
	Stopwatch<> timer;

	// The disjoint time stamp, which indicates valid timestamps and the GPU frequency
	std::array<ComPtr<ID3D11Query>, buffer_size> timestamp_disjoint;

	// The registered timestamp IDs
	std::vector<std::string> timestamp_ids;

	// An array of id_string->timestamp maps. Array size depends on the amount of buffering.
	using timestamp_map = std::unordered_map<std::string, std::pair<ComPtr<ID3D11Query>, ComPtr<ID3D11Query>>>;
	std::array<timestamp_map, buffer_size> timestamps;

	// Render times for each timestamp ID. Single frame and average times.
	std::unordered_map<std::string, f32> stage_times;
	std::unordered_map<std::string, f32> total_stage_times;
	std::unordered_map<std::string, f32> avg_stage_times;
	u32 frame_count;

	// The frame to query data from
	u8 query_frame;

	// The frame to queried data from
	i16 read_frame;
};

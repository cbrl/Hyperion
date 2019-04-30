template<typename ClockT>
Stopwatch<ClockT>::Stopwatch() {
	reset();
}


template<typename ClockT>
void Stopwatch<ClockT>::pause() {
	if (!paused) {
		pause_time = ClockT::now();
		paused = true;
	}
}


template<typename ClockT>
void Stopwatch<ClockT>::resume() {
	if (paused) {
		typename ClockT::time_point start_time = ClockT::now();

		// paused duration is used to correct the total time
		pause_duration += (start_time - pause_time);

		prev_time  = start_time;
		pause_time = ClockT::duration(0);
		paused     = false;
	}
}


template<typename ClockT>
void Stopwatch<ClockT>::reset() {
	base_time      = ClockT::now();
	prev_time      = ClockT::now();
	delta_time     = ClockT::duration(0);
	pause_duration = ClockT::duration(0);
	paused         = false;
}

template<typename ClockT>
void Stopwatch<ClockT>::tick() {
	if (paused) {
		delta_time = ClockT::duration(0);
		return;
	}

	const auto curr_time = ClockT::now();

	delta_time  = curr_time - prev_time;
	total_time += delta_time;
	prev_time   = curr_time;
}

template<typename ClockT>
template<typename PeriodT>
std::chrono::duration<f64, PeriodT> Stopwatch<ClockT>::deltaTime() const {
	return std::chrono::duration_cast<std::chrono::duration<f64, PeriodT>>(delta_time);
}

template<typename ClockT>
template<typename PeriodT>
std::chrono::duration<f64, PeriodT> Stopwatch<ClockT>::totalTime() const {
	return std::chrono::duration_cast<std::chrono::duration<f64, PeriodT>>(total_time - pause_duration);
}
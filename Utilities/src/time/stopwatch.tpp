template<typename ClockT>
Stopwatch<ClockT>::Stopwatch() {
	reset();
}


template<typename ClockT>
void Stopwatch<ClockT>::pause() {
	if (not paused) {
		pause_time = ClockT::now();
		delta_time = typename ClockT::duration(0);
		paused     = true;
	}
}


template<typename ClockT>
void Stopwatch<ClockT>::resume() {
	if (paused) {
		const auto resume_time = ClockT::now();

		pause_duration += resume_time - prev_time;

		prev_time  = resume_time;
		pause_time = typename ClockT::time_point();
		paused     = false;
	}
}


template<typename ClockT>
void Stopwatch<ClockT>::reset() {
	base_time      = ClockT::now();
	prev_time      = ClockT::now();
	total_time     = typename ClockT::duration(0);
	delta_time     = typename ClockT::duration(0);
	pause_duration = typename ClockT::duration(0);
	paused         = false;
}

template<typename ClockT>
void Stopwatch<ClockT>::tick() {
	const auto curr_time = ClockT::now();

	if (not paused)
		delta_time = curr_time - prev_time;
	else
		pause_duration += curr_time - prev_time; //pause duration is used to correct total time

	total_time = curr_time - base_time;
	prev_time  = curr_time;
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

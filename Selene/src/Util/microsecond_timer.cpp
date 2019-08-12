#include <chrono>

#include "microsecond_timer.hpp"

MicrosecondTimer::MicrosecondTimer() : should_stop_(false) {
	this->moveToThread(&thread_);
	connect(this, &MicrosecondTimer::StartTiming, this, &MicrosecondTimer::Timing);
}

void MicrosecondTimer::Start(const int microsecond) {
	thread_.start();
	emit StartTiming(microsecond);
}

void MicrosecondTimer::Stop() {
	should_stop_ = true;
}

void MicrosecondTimer::Timing(const int microsecond) {
	auto t1 = std::chrono::steady_clock::now();

	while (!should_stop_) {
		const auto t2 = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() > microsecond) {
			t1 = t2;
			emit TimeOut();
		}
	}
}
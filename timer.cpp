#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "timer.hpp"


Timer::Timer(bool initRunning ) : buffer_(0), running_(initRunning) {

}

Timer::~Timer() {

}

int Timer::getElapsedTime() const {

	if (running_) 
		return (int)(buffer_+clock_.GetElapsedTime());
	return buffer_;
}

bool Timer::isRunning() const {
	return running_;
}

void Timer::start() {
	if(!running_) {
		running_=true;
		clock_.Reset();
	}
}

void Timer::stop() {
	if (running_) {
		buffer_+= (int)clock_.GetElapsedTime();
		running_ = false;
	}
}

void Timer::restart(bool continueRunning) {
	buffer_ = 0;
	running_ = continueRunning;
	clock_.Reset();
}

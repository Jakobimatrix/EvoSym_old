#ifndef _TIMER_
#define _TIMER_
#include <chrono>

class Timer{

private:
	std::chrono::steady_clock::time_point startedTime;
	std::chrono::steady_clock::time_point stopTime;
	double passedTimeNS;
	bool started;
	bool valide;
	int loops;


public:
	Timer() {
		this->reset();
	}

	void start() {
		this->startedTime = std::chrono::steady_clock::now();
		this->started = true;
	}

	void stopLoop() {
		this->stopTime = std::chrono::steady_clock::now();
		passedTimeNS += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(stopTime - startedTime).count();
		loops++;
		this->valide = true;		
	}

	void stop() {
		this->stopTime = std::chrono::steady_clock::now();
		if (this->started) {
			this->valide = true;
		}
	}

	void reset() {
		this->valide = false;
		this->started = false;
		passedTimeNS = 0.0;
		loops = 0;
	}


	double getTime_in_ns() {
		if (this->valide) {
			if (this->passedTimeNS == 0.0) {
				return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(stopTime - startedTime).count();
			}
			else {
				return passedTimeNS;
			}			
		}
		else {
			return -1.0;
		}
	}

	double getTime_in_us() {
		if (this->valide) {
			if (this->passedTimeNS == 0.0) {
				return (double)std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startedTime).count();
			}
			else {
				return passedTimeNS / 1000;
			}
		}
		else {
			return -1.0;
		}
	}

	double getTime_in_ms() {
		if (this->valide) {
			if (this->passedTimeNS == 0.0) {
				return (double)std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startedTime).count();
			}
			else {
				return passedTimeNS / 1000000;
			}
		}
		else {
			return -1.0;
		}
	}

	int getLoops() {
		return loops;
	}

};

#endif // !_TIMER_

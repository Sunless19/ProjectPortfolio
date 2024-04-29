#pragma once
#include <iostream>;
#include <chrono>;
#include <thread>;
class StopWatch
{

private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point endTime;

public:
	StopWatch() {}
	void start();
	void stop();
	float elapsed_time();
	float run_time();

};


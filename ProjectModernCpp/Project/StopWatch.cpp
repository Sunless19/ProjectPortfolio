#include "StopWatch.h"

void gartic::Stopwatch::start()
{
	startTime = std::chrono::high_resolution_clock::now();
}

void gartic::Stopwatch::stop()
{
	endTime = std::chrono::high_resolution_clock::now();
}

float gartic::Stopwatch::elapsed_time()
{
	std::chrono::duration<float> duration = endTime - startTime;
	return duration.count();
}
float gartic::Stopwatch::run_time()
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
	return duration.count();
}
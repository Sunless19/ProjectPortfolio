#include "StopWatch.h"

void StopWatch::start()
{
	startTime = std::chrono::high_resolution_clock::now();
}

void StopWatch::stop()
{
	endTime = std::chrono::high_resolution_clock::now();
}

float StopWatch::elapsed_time()
{
	std::chrono::duration<float> duration = endTime - startTime;
	return duration.count();
}
float StopWatch::run_time()
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
	return duration.count();
}
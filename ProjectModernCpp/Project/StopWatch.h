#pragma once
import <iostream>;
import <chrono>;
import <thread>;
namespace gartic
{
    class Stopwatch
    {
    private:
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::high_resolution_clock::time_point endTime;

    public:
        Stopwatch() {}
        void start();
        void stop();
        float elapsed_time();
        float run_time();
    };
}
// DLL momentan stopwatch .

/*
 * Timer
 *
 * Usage:
 *
 * double result, result2;
 *
 * Timer timer;
 * ... do something ...
 * timer.done();
 * result = timer.result();
 * timer.reset();
 * ... do something ...
 * timer.done();
 * result2 = timer.result();
 * ...etc.
 *
 */

#pragma once

#ifndef ACCEL_CUDA

#include <chrono>

class Timer {
private: // fields:

   double data;
   std::chrono::time_point<std::chrono::high_resolution_clock> begin;

public: // ctor:

   Timer(): data(0), begin(std::chrono::high_resolution_clock::now()) {}

public: // API:

   //@ reset the timer
   void reset() {
      begin = std::chrono::high_resolution_clock::now();
   }

   //@ record time elapsed, in seconds, as a double
   void done() {
      data = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>
            (std::chrono::high_resolution_clock::now() - begin).count();
   }

   //@ the result
   [[nodiscard]] double result() const {
      return data;
   }

};


#else

// This has been giving me problems (?)

#include <cuda_runtime.h>

class Timer {
private: // fields:

    cudaEvent_t start;
    cudaEvent_t stop;
    double data;
    cudaStream_t stream;

public: // ctor:

    Timer(cudaStream_t stream):
        stream(stream)
    {
        cudaEventCreate(&start);
        cudaEventCreate(&stop);
    }

public: // API:

    //@ reset the timer
    void reset() {
        cudaEventRecord(start, 0);
    }

    //@ record time elapsed, in seconds, as a double
    void done() {
        float tmp;
        cudaEventRecord(stop, 0);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&tmp, start, stop);
        data = (double)tmp;
    }

    //@ the result
    [[nodiscard]] double result() const {
        return data;
    }

};

#endif



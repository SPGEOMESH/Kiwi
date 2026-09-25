
#pragma once


#ifndef ACCEL_CUDA

#include <cctype>
#include <chrono>

// function execution specifiers
// https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#function-declaration-specifiers
#define __global__
#define __device__
#define __host__
#define __noinline__
#define __forceinline__
// memory space specifiers
// https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#variable-memory-space-specifiers
#define __device __
#define __constant__
#define __shared__
#define __managed__
#define __restrict__
#define __local__


using cudaError_t = int;
using cudaStream_t = int;

struct cudaEvent_t {
    std::chrono::time_point<std::chrono::high_resolution_clock> now;
    cudaEvent_t(): now(std::chrono::high_resolution_clock::now()) {}
};


#endif



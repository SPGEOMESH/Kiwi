
#pragma once

#ifndef GPUrun


#include <kiwi/cuda/cuda_types_sim.h>


cudaError_t cudaEventCreate(cudaEvent_t * event, unsigned int flags = 0);

cudaError_t cudaEventRecord(cudaEvent_t & event, cudaStream_t stream = 0);

cudaError_t cudaEventSynchronize(cudaEvent_t & event);

cudaError_t cudaEventElapsedTime(float * data, cudaEvent_t & ev1, cudaEvent_t & ev2);

#endif

#pragma once

#ifndef GPUrun


#include <kiwi/cuda/cuda_types_sim.h>
#include <kiwi/cuda/cuda_events_sim.h>



__host__
cudaError_t cudaMalloc(void ** d_ptr, size_t size);

__host__
template <typename T>
void cudaFree(T * ptr) {
    free(ptr);
}

__host__
cudaError_t cudaMemset( void* d_ptr, int value, size_t count);




//---------------------------------------------------------------------------



// threadfence, threads in this block
void __threadfence_block();


// global threadfence, all blocks, all threads, only up to shared memory
void __threadfence();


// thread sync in block, applying to global memory
void __threadsync();


#endif

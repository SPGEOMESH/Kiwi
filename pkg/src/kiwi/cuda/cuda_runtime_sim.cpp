

#ifndef ACCEL_CUDA

#include <kiwi/cuda/cuda_runtime_sim.h>
#include <string>
#include <cstring>

__host__
cudaError_t cudaMalloc(void ** d_ptr, size_t size) {
    *d_ptr = malloc(size);
    return cudaError_t();
}

__host__
cudaError_t cudaMemset( void* d_ptr, int value, size_t count) {
    // void* memset( void* dest, int ch, std::size_t count );
    // Note: count counts bytes (chars) in both C++ and the CUDA analog.
    memset(d_ptr, value, count);
    return cudaError_t();
}




//---------------------------------------------------------------------------

// sync


// thread sync in block, applying to all memory accesses to global, shared.
void __threadsync() {
    ;
}



//--------------------------------------------------------------------------

// fence


/*
 * Remember:
 * a fence only guarantees that other observers
 * will see *your* memory accesses in a certain order.
 * Let X mark the thread fence, and let a,b,c,d,etc. be
 * memory accesses (reads or writes).
 * Then if you see your memory accesses as
 *     abcdefXghijklm
 * then another thread might see, ****eventually****,
 *     dcfbaeXkhmlig
 * but moreover, there is no guarantee of the time ***when***
 * the other thread sees this access pattern.
 *
 */


// threadfence, threads in this block
void __threadfence_block() {
    ;
}


// global threadfence, all blocks, all threads, only up to shared memory
void __threadfence() {
    ;
}

#endif

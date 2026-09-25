
#ifndef ACCEL_CUDA

#include <kiwi/cuda/cuda_types_sim.h>


cudaError_t cudaEventCreate(cudaEvent_t * event, unsigned int flags = 0) {
    // reset the event
    *event = cudaEvent_t();
    return cudaError_t();
}

cudaError_t cudaEventRecord(cudaEvent_t & event, cudaStream_t stream = 0) {
    // reset the event
    event = cudaEvent_t();
    return cudaError_t();
}

cudaError_t cudaEventSynchronize(cudaEvent_t & event) {
    // nothing to do in CPUsim
    return cudaError_t();
}

cudaError_t cudaEventElapsedTime(float * data, cudaEvent_t & ev1, cudaEvent_t & ev2) {
    *data = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>
            (ev2.now - ev1.now).count();
    return cudaError_t();
}


#endif


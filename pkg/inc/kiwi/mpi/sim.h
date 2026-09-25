
#pragma once



constexpr bool mpi_enabled() {
#ifdef USE_MPI
    return true;
#else
    return false;
#endif
}



#ifdef USE_MPI


#include <mpi.h>


#else

#include <kiwi/mpi/serial.h>


// ----------------------
// Defines (Legacy)
// ----------------------


// objects/lists
#define MPI_INT SERIAL_INT
#define MPI_INT32_T SERIAL_INT32_T
#define MPI_FLOAT SERIAL_FLOAT
#define MPI_DOUBLE SERIAL_DOUBLE
#define MPI_COMM_NULL SERIAL_COMM_NULL
#define MPI_COMM_WORLD SERIAL_COMM_WORLD
#define MPI_COMM_SELF SERIAL_COMM_SELF
#define MPI_UNDEFINED SERIAL_UNDEFINED
#define MPI_STATUS_IGNORE SERIAL_STATUS_IGNORE
#define MPI_STATUSES_IGNORE SERIAL_STATUSES_IGNORE
#define MPI_COMM_TYPE_SHARED SERIAL_COMM_TYPE_SHARED
#define MPI_INFO_NULL SERIAL_INFO_NULL
#define MPI_WIN_NULL SERIAL_WIN_NULL
#define MPI_SUCCESS SERIAL_SUCCESS


#define MPI_MAX SERIAL_MAX
#define MPI_MIN SERIAL_MIN


// ----------------
// Type System
// ----------------



using MPI_Comm = Serial::Comm;
using MPI_Request = Serial::Request;
using MPI_Aint = Serial::Aint;
using MPI_Op = Serial::Op;

using MPI_Win = Serial::Win;
using MPI_Datatype = Serial::Datatype;
using MPI_Info = Serial::Info;
using MPI_Status = Serial::Status;


// ----------------
// Init/Finalize
// ----------------



inline
int MPI_Init(int* argc, char*** argv) {
    return Serial::Init(argc, argv);
}

inline
bool MPI_Initialized(int* flag) {
    return Serial::Initialized(flag);
}

inline
bool MPI_Finalized(int* flag) {
    return Serial::Finalized(flag);
}

inline
int MPI_Finalize() {
    return Serial::Finalize();
}

inline
void MPI_Abort(MPI_Comm comm, int ecode) {
    Serial::Abort(comm, ecode);
}





// --------------------
// Communicators
// --------------------



inline
int MPI_Comm_dup(MPI_Comm comm, MPI_Comm* newcomm) {
    return Serial::Comm_dup(comm, newcomm);
}

inline
int MPI_Comm_rank(MPI_Comm comm, int* rank) {
    return Serial::Comm_rank(comm, rank);
}

inline
int MPI_Comm_size(MPI_Comm comm, int* size) {
    return Serial::Comm_size(comm, size);
}

inline
int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm* newcomm) {
    return Serial::Comm_split(comm, color, key, newcomm);
}


inline
int MPI_Comm_split_type(MPI_Comm comm, int split_type, int key, MPI_Info info, MPI_Comm* newcomm) {
    return Serial::Comm_split_type(comm, split_type, key, info, newcomm);
}

inline
int MPI_Comm_free(MPI_Comm* comm) {
    return Serial::Comm_free(comm);
}




// -----------------------------------------------
// Plain Memory Allocation/Deallocation
// -----------------------------------------------

inline
int MPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr) {
    return Serial::Alloc_mem(size, info, baseptr);
}

// Frees memory that has been
// allocated using MPI_Alloc_mem.
// Works exactly like malloc/free, new/delete.
inline
int MPI_Free_mem(void* baseptr) {
    return Serial::Free_mem(baseptr);
}



// ----------------------------------------
// Memory Barriers
// ----------------------------------------





inline
int MPI_Barrier(MPI_Comm comm) {
    return Serial::Barrier(comm);
}


inline
int MPI_Ibarrier(MPI_Comm comm, MPI_Request* request) {
    return Serial::Ibarrier(comm, request);
}

inline
int MPI_Barrier_init(MPI_Comm comm, MPI_Info info, MPI_Request *request) {
    return Serial::Barrier_init(comm, info, request);
}





// -----------------------------------
// Aggregation and Broadcast
// -----------------------------------


// Broadcasts a message from the process with rank root to all other processes of the group.
inline
int MPI_Bcast(
        void* buffer,
        int count,
        MPI_Datatype datatype,
        int root,
        MPI_Comm comm
) {
    return Serial::Bcast(buffer, count, datatype, root, comm);
}

// Broadcasts a message from the process with rank root to all other processes of the group.
inline
int MPI_Ibcast(
        void*buffer,
        int count,
        MPI_Datatype datatype,
        int root,
        MPI_Comm comm,
        MPI_Request* request
) {
    return Serial::Ibcast(buffer, count, datatype, root, comm, request);
}

// Gathers values from a group of processes.
inline
int MPI_Gather(
        const void *sendbuf,
        int sendcount,
        MPI_Datatype sendtype,
        void *recvbuf,
        int recvcount,
        MPI_Datatype recvtype,
        int root,
        MPI_Comm comm
) {
    return Serial::Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
}

//  Gathers values from a group of processes.
// The receive buffer is ignored for all nonroot processes.
inline
int MPI_Igather(
        const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
                MPI_Comm comm, MPI_Request *request
) {
    return Serial::Igather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request);
}

//  Gathers values from a group of processes.
// The receive buffer is ignored for all nonroot processes.
inline
int MPI_Gather_init(
        const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                    void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
                    MPI_Comm comm, MPI_Info info, MPI_Request *request
) {
    return Serial::Gather_init(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, info, request);
}


inline
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
              MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
   return Serial::Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

inline
int MPI_Iallreduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, MPI_Comm comm,
               MPI_Request *request) {
   return Serial::Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request);
}




// --------------------------------
// Two-way communication
// --------------------------------


inline
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm) {
    return Serial::Send(buf, count, datatype, dest, tag, comm);
}

inline
int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest,
          int tag, MPI_Comm comm, MPI_Request *request) {
    return Serial::Isend(buf, count, datatype, dest, tag, comm, request);
}

inline
int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
             int source, int tag, MPI_Comm comm, MPI_Status *status) {
    return Serial::Recv(buf, count, datatype, source, tag, comm, status);
}

inline
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype,
          int source, int tag, MPI_Comm comm, MPI_Request *request) {
    return Serial::Irecv(buf, count, datatype, source, tag, comm, request);
}

inline
int MPI_Waitall(int count, MPI_Request array_of_requests[],
                MPI_Status *array_of_statuses) {
   return Serial::Waitall(count, array_of_requests, array_of_statuses);
}



// -----------------------------------------
// One-way communication (RMA)
// -----------------------------------------


inline
int MPI_Win_create(void *base, MPI_Aint size, int disp_unit,
               MPI_Info info, MPI_Comm comm, MPI_Win *win) {
   return Serial::Win_create(base, size, disp_unit, info, comm, win);
}

// One-sided MPI call that allocates shared memory
// and returns a window object for RMA operations.
// NOTE: To free this shared memory, call MPI_Win_free.
// Win_allocate_shared/Win_free works exactly like malloc/free, new/delete.
inline
int MPI_Win_allocate_shared(MPI_Aint size, int disp_unit, MPI_Info info,
                            MPI_Comm comm, void *baseptr, MPI_Win *win) {
    return Serial::Win_allocate_shared(size, disp_unit, info, comm, baseptr, win);
}

inline
int MPI_Win_free(MPI_Win* winp) {
    return Serial::Win_free(winp);
}

inline
int MPI_Win_shared_query(MPI_Win win, int rank, MPI_Aint *size,
                         int *disp_unit, void *baseptr) {
    return Serial::Win_shared_query(win, rank, size, disp_unit, baseptr);
}

inline
int MPI_Win_fence(int assert, MPI_Win win) {
    return Serial::Win_fence(assert, win);
}



// Copies data from the origin memory to the target.
inline
int MPI_Put(
        const void *origin_addr,
        int origin_count,
        MPI_Datatype origin_datatype,
        int target_rank,
        MPI_Aint target_disp,
        int target_count,
        MPI_Datatype target_datatype,
        MPI_Win win
) {
    return Serial::Put(origin_addr, origin_count, origin_datatype,
        target_rank, target_disp, target_count, target_datatype, win);
}

// Copies data from the target memory to the origin.
inline
int MPI_Get(
        void *origin_addr,
        int origin_count,
        MPI_Datatype origin_datatype,
        int target_rank,
        MPI_Aint target_disp,
        int target_count,
        MPI_Datatype target_datatype,
        MPI_Win win
) {
    return Serial::Get(origin_addr, origin_count, origin_datatype,
           target_rank, target_disp, target_count, target_datatype, win);
}


// -----------------------------------------
// Miscellaneous
// -----------------------------------------







// -----------------------------------------
// END
// -----------------------------------------






#endif




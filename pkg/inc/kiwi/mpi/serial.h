
#pragma once



/*
 * Serial spoof of the MPI library.
 * Useful for testing, for example
 * testing effects of modifications to
 * intra-node computation
 * and communication strategies.
 *
 * Substitute "Serial" with "MPI" and
 * vice versa is the universal rule.
 * For defines where objects appear in all caps,
 * "SERIAL" is used instead.
 */

#include <cstdlib>
#include <cstdint>
#include <cstring>


// ----------------------
// Defines (Legacy)
// ----------------------


#define SERIAL_INT Serial::int32
#define SERIAL_INT32_T Serial::int32
#define SERIAL_DOUBLE Serial::real64
#define SERIAL_FLOAT Serial::real32
#define SERIAL_COMM_NULL Serial::Comm_null
#define SERIAL_COMM_WORLD Serial::Comm_world
#define SERIAL_COMM_SELF Serial::Comm_self
#define SERIAL_UNDEFINED (1)
#define SERIAL_STATUS_IGNORE (&Serial::Status_ignore)
#define SERIAL_STATUSES_IGNORE (&Serial::Statuses_ignore)
#define SERIAL_COMM_TYPE_SHARED (1)
#define SERIAL_INFO_NULL Serial::Info_null
#define SERIAL_WIN_NULL Serial::Win_null
#define SERIAL_SUCCESS (0)
#define SERIAL_ERR_NO_MEM (-1)
#define SERIAL_ERR_ARG (-1)

#define SERIAL_MAX (0)
#define SERIAL_MIN (0)



namespace Serial {


// ----------------
// Type System
// ----------------

using Request = int32_t*;
using Aint = size_t;
using Op = int32_t;

struct Comm {
    int id;
    bool operator==(Comm& other) {
        return (id == other.id);
    }
    bool operator!=(Comm& other) {
        return !operator==(other);
    }
};

struct Win {
    void* baseptr;
    size_t disp_unit;

    bool operator==(Win& other) {
        return (baseptr == other.baseptr);
    }
    bool operator!=(Win& other) {
        return !operator==(other);
    }
};

struct Datatype {
    size_t size;
};

struct Info {
    size_t info;
};

struct Status {
    size_t status;
};



// In C++ and later the `inline` keyword prevents
// double-instantiation (ODR rule).
inline Comm Comm_null = {0};
inline Comm Comm_world = {1};
inline Comm Comm_self = {2};
inline Info Info_null = {};
inline Win Win_null = {nullptr};
inline Datatype int32 = {4};
inline Datatype int64 = {8};
inline Datatype real32 = {4};
inline Datatype real64 = {8};
inline Status Status_ignore = {0};
inline Status Statuses_ignore = {0};


struct SerialState {

   int is_initialized = 0;

   // todo irecv/isend

};

inline
SerialState serialstate;


// ----------------
// Init/Finalize
// ----------------


inline
int Init(int *argc, char ***argv) {
   serialstate.is_initialized = 1;
   return 0;
}

inline
int Initialized(int *flag) {
   *flag = serialstate.is_initialized;
    return 0;
}

inline
int Finalized(int* flag) {
   *flag = !serialstate.is_initialized;
   return 0;
}

inline
int Finalize() {
   serialstate.is_initialized = 0;
   return 0;
}

inline
void Abort(Comm, int) {
   exit(-1);
}



// --------------------
// Communicators
// --------------------


inline
int Comm_dup(Comm, Comm*) {
   return 0;
}

inline
int Comm_rank(Comm, int* rank) {
   *rank = 0;
   return 0;
}

inline
int Comm_size(Comm, int* size) {
   *size = 1;
   return 0;
}

inline
int Comm_split(Comm, int, int, Comm*) {
   return 0;
}

inline
int Comm_split_type(Comm, int, int, Serial::Info, Comm*) {
   return 0;
}

inline
int Comm_free(Comm*) {
   return 0;
}




// -----------------------------------------------
// Plain Memory Allocation/Deallocation
// -----------------------------------------------



inline
int Alloc_mem(Aint size, Info info, void* baseptr) {
   if (baseptr == nullptr) {
      return SERIAL_ERR_ARG;
   }
   // Create a pointer that points to nothing
   void* ptr;
   // Allocate size bytes of memory on the heap
   ptr = malloc((size_t)size);
   if (ptr == nullptr && size > 0) {
      return SERIAL_ERR_ARG;
   }
   // Cast baseptr to void** so we can write the allocated address
   *(void**)baseptr = ptr;
   return SERIAL_SUCCESS;
}

inline
int Free_mem(void* baseptr) {
   free(baseptr);
    return SERIAL_SUCCESS;
}


// -----------------------------------
// Memory Barriers
// -----------------------------------




inline
int Barrier(Comm) {
    return 0;
}

inline
int Ibarrier(Comm, Request*) {
    return 0;
}

inline
int Barrier_init(Comm, Info, Request*) {
    return 0;
}



// -----------------------------------
// Aggregation and Broadcast
// -----------------------------------




inline
int Bcast(void*, int, Datatype, int, Comm) {
    return 0;
}


inline
int Ibcast(void*, int, Datatype, int, Comm, Request*) {
    return 0;
}

inline
int Gather(const void*, int, Datatype, void*, int, Datatype, int, Comm) {
    return 0;
}

inline
int Igather(const void*, int, Datatype, void*, int, Datatype, int, Comm, Request*) {
    return 0;
}

inline
int Gather_init(const void*, int, Datatype, void*, int, Datatype, int, Comm, Info, Request*) {
    return 0;
}

inline
int Allreduce(const void *sendbuf, void *recvbuf, int count,
              Datatype datatype, Op op, Comm comm) {
   auto x = reinterpret_cast<const Datatype*>(sendbuf);
   auto y = reinterpret_cast<Datatype*>(recvbuf);
   for (int i = 0; i < count; ++i) {
      y[i] = x[i];
   }
   return 0;
}

inline
int Iallreduce(const void *sendbuf, void *recvbuf, int count,
                   Datatype datatype, Op op, Comm comm,
                   Request *request) {
   return Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}



// --------------------------------
// Two-way communication
// --------------------------------


// Send/Recv patterns fill another rank's buffer image
// in that rank's address space.

// TODO
//  There is nothing to do in serial mode because
//  there is only one process, but the logic doesn't
//  serialize easily, unfortunately, unless you pass
//  the same buffer to send and receive.
//
// Note: calling Isend and Irecv on the same buffer
// when the two sides are distinct processes does
// not lead to data races. However, calling Isend
// and Irecv on the same buffer when the two sides
// are the *same* process *does* lead to data races.





inline
int Send(const void *buf, int count, Datatype datatype, int dest,
          int tag, Comm comm) {
    return 0;
}

inline
int Isend(const void *buf, int count, Datatype datatype, int dest,
          int tag, Comm comm, Request *request) {
    return 0;
}

inline
int Recv(void *buf, int count, Datatype datatype,
         int source, int tag, Comm comm, Status *status) {
   if (status != &Status_ignore)
      status->status = 0;
   return 0;
}

inline
int Irecv(void *buf, int count, Datatype datatype,
          int source, int tag, Comm comm, Request *request) {
    return 0;
}

inline
int Waitall(int count, Request array_of_requests[],
                Status *array_of_statuses) {
   return 0;
}









// -----------------------------------------
// One-way communication (RMA)
// -----------------------------------------



inline
int Win_create(void *base, Aint size, int disp_unit,
               Info info, Comm comm, Win *win) {
   int err = Alloc_mem(size, info, base);
   win->baseptr = *(void**)base;
   win->disp_unit = disp_unit;
   return err;
}

inline
int Win_allocate_shared(Aint size, int disp_unit, Info info,
                         Comm comm, void* baseptr, Win *win) {
   // Validate that output pointers are valid memory addresses
   if (baseptr == nullptr || win == nullptr) {
      return SERIAL_ERR_ARG;
   }
   // Create a pointer that points to no type
   void* ptr;
   // Allocate the requested memory on the heap
   ptr = malloc((size_t)size);
   if (ptr == nullptr && size > 0) {
      *(void **)baseptr = nullptr;
      win->baseptr = nullptr;
      return SERIAL_ERR_NO_MEM;
   }
   // Assign the heap address into the location baseptr points to
   *(void**)baseptr = ptr;
   // In serial, the MPI_Win object simply stores the pointer handle
   win->baseptr = ptr;
   return SERIAL_SUCCESS;
}

inline
int Win_free(Win* winp) {
   return Free_mem(winp->baseptr);
}

inline
int Win_shared_query(Win win, int rank, Aint *size,
                     int *disp_unit, void *baseptr) {
    // nothing to do for a serial program.
    return 0;
}

inline
int Win_fence(int assert, Win win) {
    // nothing to do for a serial program.
    return 0;
}





// Copies data from the origin memory to the target.
inline
int Put(
        const void* origin_addr,
        int origin_count,
        Datatype origin_datatype,
        int target_rank,
        Aint target_disp,
        int target_count,
        Datatype target_datatype,
        Win win
) {
    std::memcpy(const_cast<void*>(origin_addr), win.baseptr, target_count*target_datatype.size);
    return 0;
}

// Copies data from the target memory to the origin.
inline
int Get(
        void* origin_addr,
        int origin_count,
        Datatype origin_datatype,
        int target_rank,
        Aint target_disp,
        int target_count,
        Datatype target_datatype,
        Win win
) {
    std::memcpy(win.baseptr, origin_addr, target_count*target_datatype.size);
    return 0;
}




// -----------------------------------------
// Miscellaneous
// -----------------------------------------







// -----------------------------------------
// END
// -----------------------------------------


}



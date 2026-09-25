
#pragma once

#include <Tbasis.h>

#include <kiwi/mpi/sim.h>


/*
 * This tests initialization, defining a shared memory block,
 * a node communicator, a subcommunicator,
 * a basic utilization, and shutdown.
 *
 *
 */
class TMPI {

public:

    struct MyStruct {
        int id;
        double data[10];
    };

    static void Tdryrun(int argc, char** argv) {
        MPI_Init(NULL, NULL);

        MPI_Comm node_comm;
        MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &node_comm);

        int node_rank;
        MPI_Comm_rank(node_comm, &node_rank);

        MyStruct* raw_ptr = nullptr;
        MPI_Win win;

        // Allocate 100 structs on rank 0, 0 bytes on others
        MPI_Aint bytes = (node_rank == 0) ? sizeof(MyStruct) * 100 : 0;

        int err = MPI_Win_allocate_shared(bytes, sizeof(MyStruct), MPI_INFO_NULL, node_comm, &raw_ptr, &win);
        if (err != MPI_SUCCESS) {
           std::cout << "err " << err << std::endl;
            std::cerr << "Allocation failed on rank " << node_rank << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        MyStruct* basep = static_cast<MyStruct*>(raw_ptr);

        // Non-zero ranks query rank 0's base pointer
        if (node_rank != 0) {
            MPI_Aint size;
            int disp;
            MPI_Win_shared_query(win, 0, &size, &disp, raw_ptr);
            basep = static_cast<MyStruct*>(raw_ptr);
        }

        // Test writing/reading
        if (node_rank == 0) {
            basep[0].id = 777;
        }

        MPI_Barrier(node_comm);

        if (node_rank != 0) {
            std::cout << "Rank " << node_rank << " read id: " << basep[0].id << std::endl;
        }

        cout << "[Proc" << node_rank << "] number " << basep[0].id << endl;

        MPI_Win_free(&win);
        MPI_Finalize();
    }




    static void Tsend_recv(int argc, char** argv) {
        // Author: Wes Kendall
        // Copyright 2011 www.mpitutorial.com
        // This code is provided freely with the tutorials on mpitutorial.com.

        MPI_Init(NULL, NULL);
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        int number;
        if (world_rank == 0) {
            // If we are rank 0, set the number to -1 and send it to process 1
            number = 777;
            MPI_Send(
                    /* data         = */ &number,
                    /* count        = */ 1,
                    /* datatype     = */ MPI_INT,
                    /* destination  = */ 1,
                    /* tag          = */ 0,
                    /* communicator = */ MPI_COMM_WORLD);
        }
        else if (world_rank == 1) {
            MPI_Recv(
                    /* data         = */ static_cast<void*>(&number),
                    /* count        = */ 1,
                    /* datatype     = */ MPI_INT,
                    /* source       = */ 0,
                    /* tag          = */ 0,
                    /* communicator = */ MPI_COMM_WORLD,
                    /* status       = */ MPI_STATUS_IGNORE);
        }
        cout << "[Proc" << world_rank << "] number " << number << endl;
        MPI_Finalize();
    }



    static void Tdryrun_splitnode(int argc, char** argv)  {
        MPI_Init(&argc, &argv);
        int world_rank = -1, world_size = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        {
            // 1. Establish top-level node-level shared communicator (DO NOT NEST THIS CALL)
            MPI_Comm node_comm = MPI_COMM_NULL;
            MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, world_rank, MPI_INFO_NULL, &node_comm);

            int node_rank = -1, node_size = -1;
            MPI_Comm_rank(node_comm, &node_rank);
            MPI_Comm_size(node_comm, &node_size);

            // 2. Split node_comm safely into sub-teams using standard MPI_Comm_split
            // Example: Group ranks into even and odd sub-communicators
            int sub_color = node_rank % 2;
            MPI_Comm sub_comm = MPI_COMM_NULL;
            MPI_Comm_split(node_comm, sub_color, node_rank, &sub_comm);

            int sub_rank = -1, sub_size = -1;
            MPI_Comm_rank(sub_comm, &sub_rank);
            MPI_Comm_size(sub_comm, &sub_size);

            // Synchronize state across node before window setup
            MPI_Barrier(node_comm);

            // 3. Allocate shared memory window on the node
            // Only node_rank 0 allocates physical memory; others request 0 bytes
            MPI_Aint bytes = 0;
            int disp_unit = sizeof(int);
            if (node_rank == 0) {
                bytes = static_cast<MPI_Aint>(sizeof(int));
            }

            int* shared_buffer = nullptr;
            MPI_Win win = MPI_WIN_NULL;

            // Create the shared window across node_comm
            MPI_Win_allocate_shared(bytes, disp_unit, MPI_INFO_NULL, node_comm, &shared_buffer, &win);

            // Query the shared pointer allocated by rank 0 on all ranks
            int* ptr_to_rank0 = nullptr;
            MPI_Aint win_size = 0;
            int win_disp = 0;
            MPI_Win_shared_query(win, 0, &win_size, &win_disp, &ptr_to_rank0);

            // 4. Lock/Fence, write the value 42, and synchronize
            MPI_Win_fence(0, win);
            if (node_rank == 0 && shared_buffer != nullptr) {
                *shared_buffer = 42;
            }
            MPI_Win_fence(0, win);

            // Every rank reads the value written by rank 0 via shared memory
            std::cout << "[World Rank " << world_rank
                      << " | Node Rank " << node_rank
                      << " | Sub Rank " << sub_rank << "] Read value: "
                      << *ptr_to_rank0 << std::endl;

            // Synchronize all ranks before tear-down
            MPI_Barrier(MPI_COMM_WORLD);

            // 5. TEAR DOWN ORDER (CRITICAL TO PREVENT FREE-AFTER-FINALIZE CRASHES)
            // Rule: Windows MUST be freed before the communicators used to create them!
            if (win != MPI_WIN_NULL) {
                MPI_Win_free(&win);
            }

            if (sub_comm != MPI_COMM_NULL) {
                MPI_Comm_free(&sub_comm);
            }

            if (node_comm != MPI_COMM_NULL) {
                MPI_Comm_free(&node_comm);
            }
        } // All local stack variables go out of scope HERE before MPI_Finalize

        MPI_Finalize();

    }


};

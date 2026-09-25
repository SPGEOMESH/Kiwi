

#pragma once



template<int x = 0>
int sched_getcpu() {return 0;}
template<int x = 0>
int numa_node_of_cpu(int) {return 0;}
template<int x = 0>
struct bitmask*  numa_allocate_cpumask() {return nullptr;}
template<int x = 0>
void numa_node_to_cpus(int, struct bitmask*) {}
template<int x = 0>
int numa_bitmask_weight(struct bitmask*) {}
template<int x = 0>
void numa_free_cpumask(struct bitmask*) {}
template<int x = 0>
int numa_max_node() {return 0;}
template<typename Data>
void numa_tonode_memory(Data, int, int) {}


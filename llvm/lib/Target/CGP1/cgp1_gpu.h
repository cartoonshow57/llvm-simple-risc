// CGP1 GPU Kernel Header
// Include this file to use GPU intrinsics in C/C++ programs
#ifndef CGP1_GPU_H
#define CGP1_GPU_H

// Thread indexing - get current thread's position within block
static inline int cgp1_thread_id_x(void) {
    int result;
    __asm__ __volatile__("thread_id_x\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_thread_id_y(void) {
    int result;
    __asm__ __volatile__("thread_id_y\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_thread_id_z(void) {
    int result;
    __asm__ __volatile__("thread_id_z\t%0" : "=r"(result));
    return result;
}

// Block indexing - get current block's position within grid
static inline int cgp1_block_id_x(void) {
    int result;
    __asm__ __volatile__("block_id_x\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_block_id_y(void) {
    int result;
    __asm__ __volatile__("block_id_y\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_block_id_z(void) {
    int result;
    __asm__ __volatile__("block_id_z\t%0" : "=r"(result));
    return result;
}

// Block dimensions - number of threads per block
static inline int cgp1_block_dim_x(void) {
    int result;
    __asm__ __volatile__("block_dim_x\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_block_dim_y(void) {
    int result;
    __asm__ __volatile__("block_dim_y\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_block_dim_z(void) {
    int result;
    __asm__ __volatile__("block_dim_z\t%0" : "=r"(result));
    return result;
}

// Grid dimensions - number of blocks in grid
static inline int cgp1_grid_dim_x(void) {
    int result;
    __asm__ __volatile__("grid_dim_x\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_grid_dim_y(void) {
    int result;
    __asm__ __volatile__("grid_dim_y\t%0" : "=r"(result));
    return result;
}

static inline int cgp1_grid_dim_z(void) {
    int result;
    __asm__ __volatile__("grid_dim_z\t%0" : "=r"(result));
    return result;
}

// Synchronization - wait for all threads in block
static inline void cgp1_barrier(void) {
    __asm__ __volatile__("barrier");
}

// Convenience macros for common patterns
#define CGP1_GLOBAL_ID_X() (cgp1_block_id_x() * cgp1_block_dim_x() + cgp1_thread_id_x())
#define CGP1_GLOBAL_ID_Y() (cgp1_block_id_y() * cgp1_block_dim_y() + cgp1_thread_id_y())
#define CGP1_GLOBAL_ID_Z() (cgp1_block_id_z() * cgp1_block_dim_z() + cgp1_thread_id_z())

#endif // CGP1_GPU_H

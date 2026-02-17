// CGP1 GPU Kernel using builtins
// Compile with: clang -target cgp1 -S gpu_kernel.c

int get_thread_id(void) {
    return __builtin_cgp1_thread_id_x();
}

int get_block_id(void) {
    return __builtin_cgp1_block_id_x();
}

int get_combined_id(void) {
    int tid = __builtin_cgp1_thread_id_x();
    int bid = __builtin_cgp1_block_id_x();
    return tid + bid;
}

void sync_threads(void) {
    __builtin_cgp1_barrier();
}

// Test conditional branch
int conditional_kernel(int x) {
    if (x > 0) {
        return x + 1;
    } else {
        return x - 1;
    }
}

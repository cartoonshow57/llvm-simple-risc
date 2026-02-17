// Test global ID calculation
int get_global_id(void) {
    int tid = __builtin_cgp1_thread_id_x();
    int bid = __builtin_cgp1_block_id_x();
    int bdim = __builtin_cgp1_block_dim_x();
    return bid * bdim + tid;
}

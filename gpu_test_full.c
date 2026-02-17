// CGP1 GPU Full Feature Test
// Compile with: clang -target cgp1 -S gpu_test_full.c

// === Thread/Block Indexing ===
int get_global_id(void) {
    int tid = __builtin_cgp1_thread_id_x();
    int bid = __builtin_cgp1_block_id_x();
    int bdim = __builtin_cgp1_block_dim_x();
    return bid * bdim + tid;
}

// === Integer Arithmetic (MUL test) ===
int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    return a / b;
}

int modulo(int a, int b) {
    return a % b;
}

// === Bitwise Operations ===
int bitwise_ops(int a, int b) {
    int r = a & b;
    r = r | (a ^ b);
    r = r << 2;
    r = r >> 1;
    return r;
}

// === Conditional Branches ===
int conditional(int x) {
    if (x > 0) {
        return x * 2;
    } else {
        return x - 1;
    }
}

// === While Loop ===
int loop_sum(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

// === Synchronization ===
void sync_block(void) {
    __builtin_cgp1_barrier();
}

// === Shared Memory ===
int shared_mem_test(int idx) {
    __builtin_cgp1_shared_store(idx * 4, 42);
    __builtin_cgp1_barrier();
    return __builtin_cgp1_shared_load(idx * 4);
}

// === Warp Primitives ===
int warp_test(int val) {
    int ws = __builtin_cgp1_warp_size();
    int lane = __builtin_cgp1_lane_id();
    int all = __builtin_cgp1_warp_all(val > 0);
    int any = __builtin_cgp1_warp_any(val > 0);
    return ws + lane + all + any;
}

int warp_shuffle_test(int val, int src) {
    return __builtin_cgp1_warp_shuffle(val, src);
}

// === Graphics ===
void draw_pixel(int x, int y, int color) {
    __builtin_cgp1_write_pixel(x, y, color);
}

void clear(int color) {
    __builtin_cgp1_clear_screen(color);
}

// === Fast Math ===
float fast_math_test(float x) {
    float r = __builtin_cgp1_rcp(x);
    r = r + __builtin_cgp1_rsqrt(x);
    r = r + __builtin_cgp1_sin(x);
    r = r + __builtin_cgp1_cos(x);
    return r;
}

// === Floating Point ===
float fp_test(float a, float b) {
    float sum = a + b;
    float prod = a * b;
    return sum + prod;
}

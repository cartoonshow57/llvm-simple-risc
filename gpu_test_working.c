// CGP1 GPU Working Feature Test
// Compile with: clang -target cgp1 -S gpu_test_working.c

// === Thread/Block Indexing with MUL ===
int get_global_id(void) {
    int tid = __builtin_cgp1_thread_id_x();
    int bid = __builtin_cgp1_block_id_x();
    int bdim = __builtin_cgp1_block_dim_x();
    return bid * bdim + tid;
}

// === Integer Arithmetic ===
int arithmetic(int a, int b) {
    int sum = a + b;
    int diff = a - b;
    int prod = a * b;
    int quot = a / b;
    int rem = a % b;
    return sum + diff + prod + quot + rem;
}

// === Bitwise Operations ===
int bitwise(int a, int b) {
    int r = a & b;
    r = r | (a ^ b);
    r = r << 2;
    r = r >> 1;
    return r;
}

// === Conditional (if/else) ===
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
void shared_store(int idx, int val) {
    __builtin_cgp1_shared_store(idx * 4, val);
}

int shared_load(int idx) {
    return __builtin_cgp1_shared_load(idx * 4);
}

// === Warp Primitives ===
int get_warp_info(void) {
    int ws = __builtin_cgp1_warp_size();
    int lane = __builtin_cgp1_lane_id();
    return ws + lane;
}

int warp_vote(int pred) {
    return __builtin_cgp1_warp_all(pred);
}

int warp_shfl(int val, int src) {
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
float fast_rcp(float x) {
    return __builtin_cgp1_rcp(x);
}

float fast_rsqrt(float x) {
    return __builtin_cgp1_rsqrt(x);
}

float fast_sincos(float x) {
    return __builtin_cgp1_sin(x) + __builtin_cgp1_cos(x);
}

// === Floating Point ===
float fp_ops(float a, float b) {
    return a + b + a * b;
}

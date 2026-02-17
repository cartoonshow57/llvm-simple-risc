// Comprehensive CGP1 GPU Kernel Test
// Tests all features: GPU intrinsics, graphics, math, comparisons, loops

// GPU thread/block indexing
int get_global_id(void) {
    int tid = __builtin_cgp1_thread_id_x();
    int bid = __builtin_cgp1_block_id_x();
    int bdim = __builtin_cgp1_block_dim_x();
    return bid * bdim + tid;
}

// Comparison test (SETCC fix verification)
int compare_values(int a, int b) {
    int eq = (a == b);
    int ne = (a != b);
    int lt = (a < b);
    int gt = (a > b);
    return eq + ne + lt + gt;
}

// Loop test with conditional
int sum_to_n(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

// Fast math operations
float compute_lighting(float x, float y) {
    float dist = __builtin_cgp1_rsqrt(x * x + y * y);
    float angle = __builtin_cgp1_sin(x) * __builtin_cgp1_cos(y);
    return dist * angle;
}

// Warp-level operations
int warp_reduce(int val) {
    int size = __builtin_cgp1_warp_size();
    int lane = __builtin_cgp1_lane_id();
    int shuffled = __builtin_cgp1_warp_shuffle(val, lane);
    return shuffled;
}

// Shared memory operations
void shared_mem_test(int idx, int value) {
    __builtin_cgp1_shared_store(idx, value);
    __builtin_cgp1_barrier();
    int loaded = __builtin_cgp1_shared_load(idx);
}

// OpenGL-like drawing - draw a colored triangle
void draw_colored_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) {
    __builtin_cgp1_set_color(color);
    __builtin_cgp1_begin(2);  // Triangle mode
    __builtin_cgp1_vertex2i(x1, y1);
    __builtin_cgp1_vertex2i(x2, y2);
    __builtin_cgp1_vertex2i(x3, y3);
    __builtin_cgp1_end();
}

// Draw a rectangle with transforms
void draw_transformed_rect(float tx, float ty, float scale, int x, int y, int w, int h) {
    __builtin_cgp1_load_identity();
    __builtin_cgp1_translate(tx, ty, tx);
    __builtin_cgp1_scale(scale, scale, scale);
    __builtin_cgp1_set_color(0x00FF00);
    __builtin_cgp1_fill_rect(x, y, w, h);
}

// Texture sampling
int sample_and_blend(float u, float v, int base_color) {
    __builtin_cgp1_bind_texture(0);
    int tex_color = __builtin_cgp1_tex2d(u, v);
    __builtin_cgp1_set_blend_mode(1);  // Alpha blend
    return tex_color;
}

// Main render kernel
void render_kernel(int frame) {
    // Clear screen
    __builtin_cgp1_clear_screen(0x000000);
    
    // Setup viewport
    __builtin_cgp1_set_viewport(0, 0, 640, 480);
    
    // Get thread ID
    int gid = get_global_id();
    
    // Draw based on thread ID
    int x = (gid % 10) * 64;
    int y = (gid / 10) * 48;
    
    // Draw shapes
    __builtin_cgp1_set_color(0xFF0000);
    __builtin_cgp1_fill_rect(x, y, 60, 44);
    
    __builtin_cgp1_set_color(0x0000FF);
    __builtin_cgp1_draw_circle(x + 30, y + 22, 20);
    
    // Synchronize
    __builtin_cgp1_barrier();
    
    // Swap buffers
    __builtin_cgp1_swap_buffers();
}

// Pixel shader test
int pixel_shader(int x, int y, float time) {
    float fx = (float)x;
    float fy = (float)y;
    
    // Compute some lighting
    float light = compute_lighting(fx, fy);
    
    // Read existing pixel
    int existing = __builtin_cgp1_read_pixel(x, y);
    
    return existing;
}

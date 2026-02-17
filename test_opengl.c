// Test OpenGL-like graphics functions for CGP1 GPU

// Draw a simple triangle
void draw_triangle_test(void) {
    __builtin_cgp1_set_color(0xFF0000);  // Red
    __builtin_cgp1_begin(2);  // Triangle mode
    __builtin_cgp1_vertex2i(100, 100);
    __builtin_cgp1_vertex2i(200, 100);
    __builtin_cgp1_vertex2i(150, 50);
    __builtin_cgp1_end();
}

// Draw shapes with different colors
void draw_shapes(void) {
    // Blue filled rectangle
    __builtin_cgp1_set_color(0x0000FF);
    __builtin_cgp1_fill_rect(10, 10, 50, 50);
    
    // Green circle outline
    __builtin_cgp1_set_color(0x00FF00);
    __builtin_cgp1_draw_circle(100, 100, 30);
    
    // Yellow filled circle
    __builtin_cgp1_set_color(0xFFFF00);
    __builtin_cgp1_fill_circle(200, 100, 25);
}

// Setup viewport and transforms
void setup_scene(float tx, float ty, float sx, float sy, float angle) {
    __builtin_cgp1_set_viewport(0, 0, 640, 480);
    __builtin_cgp1_load_identity();
    __builtin_cgp1_translate(tx, ty, tx);
    __builtin_cgp1_scale(sx, sy, sx);
    __builtin_cgp1_rotate_z(angle);
}

// Texture mapping test
int sample_texture(float u, float v) {
    __builtin_cgp1_bind_texture(0);
    return __builtin_cgp1_tex2d(u, v);
}

// Read pixel test
int get_pixel(int x, int y) {
    return __builtin_cgp1_read_pixel(x, y);
}

// Main render function
void render(float tx, float ty) {
    __builtin_cgp1_clear_screen(0x000000);  // Black background
    setup_scene(tx, ty, tx, ty, tx);
    draw_triangle_test();
    draw_shapes();
    __builtin_cgp1_swap_buffers();
}

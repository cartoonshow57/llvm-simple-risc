// Test float constant support
void test_floats(void) {
    __builtin_cgp1_translate(100.0f, 200.0f, 0.0f);
    __builtin_cgp1_scale(2.0f, 2.0f, 1.0f);
    __builtin_cgp1_rotate_z(0.5f);
}

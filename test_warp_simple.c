// Test warp without comparison
int warp_simple_test(int val) {
    return __builtin_cgp1_warp_all(val);
}

// Test warp predicates
int warp_pred_test(int val) {
    return __builtin_cgp1_warp_all(val > 0);
}

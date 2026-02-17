// Test fast math
float rcp_test(float x) {
    return __builtin_cgp1_rcp(x);
}

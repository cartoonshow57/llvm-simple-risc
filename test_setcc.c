// Test SETCC issue - comparing and storing result
int compare_gt(int a, int b) {
    return a > b;  // This requires SETCC to produce 0 or 1
}

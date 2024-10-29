double copysign(double x, double y) {
    *(int*)&x = (*(int*)&x) & 0x7FFFFFFF | (*(int*)&y) & 0x80000000;
    return x;
}
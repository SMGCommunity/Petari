#pragma once

class JUTException {
public:
    static void panic_f(const char *, int, const char *, ...);
};

#include "Python.h"

#ifndef PLATFORM
#define PLATFORM "unknown"
#endif

const char *
Py_GetPlatform(void)
{
    return PLATFORM;
}

const char *
Py_GetCheriPlatform(void) {
#ifdef __CHERI_PURE_CAPABILITY__
#if defined(__aarch64__)
    return "c128m";
#elif defined(__riscv64__)
    return "c128";
#elif defined(__riscv32__)
    return "c64";
#else
    #error "Unknown CHERI platform"
#endif
#else
    return "nocheri"
#endif
}

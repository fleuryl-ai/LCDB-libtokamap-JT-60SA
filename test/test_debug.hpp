// test/test_debug.hpp

#pragma once

#ifdef DEBUG_TEST
    #include <iostream>

    #define TEST_DEBUG(x) \
        do { std::cout << x << std::endl; } while (0)
#else
    #define TEST_DEBUG(x) \
        do { } while (0)
#endif

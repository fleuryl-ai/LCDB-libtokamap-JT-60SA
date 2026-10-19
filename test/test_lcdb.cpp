#include "lcdb/lcdb_c_api.hpp"
#include "test_config.hpp"
#include "test_debug.hpp"

#include <dlfcn.h>
#include <cassert>
#include <iostream>

void test_load()
{
    void* handle = dlopen("/analysis/lib/liblcdb.so", RTLD_NOW);

    if (handle == nullptr)
    {
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
    }

    TEST_DEBUG("liblcdb.so loaded successfully");

    dlclose(handle);
}

void test_open()
{
    int rc = lcdbOpen(TestConfig::root, TestConfig::shotno, TestConfig::category);

    TEST_DEBUG("rc=" << rc);

    assert(rc == 0);

    lcdbClose(TestConfig::root, TestConfig::shotno, TestConfig::category);
}

void test_close()
{
    lcdbOpen(TestConfig::root, TestConfig::shotno, TestConfig::category);

    int rc = lcdbClose(TestConfig::root, TestConfig::shotno, TestConfig::category);

    TEST_DEBUG("rc=" << rc);

    assert(rc == 0);
}

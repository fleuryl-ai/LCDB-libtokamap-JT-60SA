#include "lcdb/lcdb_client.hpp"
#include "lcdb/lcdb_types.hpp"
#include "test_config.hpp"
#include "test_debug.hpp"
#include "libtokamap/libtokamap.hpp"

#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

void test_lcdb_open_close()
{
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);
    client.open_session(TestConfig::category);
    client.close_session(TestConfig::category);
}

void test_lcdb_duplicate_open()
{
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);
    client.open_session(TestConfig::category);

    bool caught = false;

    try
    {
        client.open_session(TestConfig::category);
    }
    catch (const std::runtime_error&)
    {
        caught = true;
    }

    assert(caught);
}

void test_lcdb_close_unopened()
{
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);

    bool caught = false;

    try
    {
        client.close_session(TestConfig::category);
    }
    catch (const std::runtime_error&)
    {
        caught = true;
    }

    assert(caught);
}

void test_lcdb_close_all()
{
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);
    client.open_session(TestConfig::category);
    client.open_session(TestConfig::category2);

    client.close_all();

    bool caught = false;

    try
    {
        client.close_session(TestConfig::category);
    }
    catch (const std::runtime_error&)
    {
        caught = true;
    }

    assert(caught);
}

void test_lcdb_get_value()
{
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);
    client.open_session(TestConfig::category);

    // scalar string
    libtokamap::TypedDataArray device = client.get_value(TestConfig::category, "DEVICE");

    const char* device_str = device.data<char>();
    assert(std::string(device_str) == "JT60SA");
    // TEST_DEBUG("string value : " + std::string(device_str));

    // scalar int
    libtokamap::TypedDataArray shotno = client.get_value(TestConfig::category, "SHTNO");
    const int* shotno_int = shotno.data<int>();
    assert(shotno_int[0] == 75000);

    // scalar double
    libtokamap::TypedDataArray t2 = client.get_value(TestConfig::category, "T2");
    const double* t2_double = t2.data<double>();
    // T2=0.01000000000000e+02,
    assert(t2_double[0] == 1.0);

    // array int
    libtokamap::TypedDataArray nexpdata = client.get_value(TestConfig::category, "NEXPDATA");
    auto nexpdata_array = nexpdata.to_vector<int>();
    auto nexpdata_shape = nexpdata.shape();

    // nexpdata_array.print();

    // shape
    assert(nexpdata_shape[0] == 3);
    assert(nexpdata_shape[1] == 1);
    // data
    assert(nexpdata_array[0] == 14);
    assert(nexpdata_array[1] == 2);
    assert(nexpdata_array[2] == 14);

    // array double
    libtokamap::TypedDataArray xexpdata = client.get_value(TestConfig::category, "XEXPDATA");
    // debug
    TEST_DEBUG(xexpdata.to_string());

    auto xexpdata_array = xexpdata.to_vector<double>();
    auto xexpdata_shape = xexpdata.shape();

    // shape
    assert(xexpdata_shape[0] == 14);
    assert(xexpdata_shape[1] == 3);
    assert(xexpdata_shape[2] == 1);
    // data
    assert(xexpdata_array[0] == 7.42544003597721e-01);
    assert(xexpdata_array[3] == 7.17403463764677e-01);
    assert(xexpdata_array[12] == 0.684514767245105);
    assert(xexpdata_array[14] == 9.24338147952912e-01);
    // dummy data 0.0
    // data count = 14,2,14 -> second rank EXTIME have only 2points. After 3point is zero.
    assert(xexpdata_array[7] == 0.0);

    client.close_session(TestConfig::category);
}

void test_lcdb_get_value_string_array()
{
    // array string
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);
    client.open_session(TestConfig::category);

    bool caught = false;

    try
    {
        libtokamap::TypedDataArray exdb = client.get_value(TestConfig::category, "EXDB");
        auto exdb_array = exdb.to_vector<char>();
        auto exdb_shape = exdb.shape();
        // debug
        TEST_DEBUG(exdb.to_string());

        assert(exdb_shape.size() == 2);
        assert(exdb_shape[0] == 1);
        assert(exdb_shape[1] == 13);

        std::string value(exdb_array.data());

        assert(value == "YAGTS/necore");
    }
    catch (const std::runtime_error&)
    {

        caught = true;
    }

    assert(caught == false);

    client.close_session(TestConfig::category);
}

void test_lcdb_get_value_unopend()
{
    lcdb::LcdbClient client(TestConfig::root, TestConfig::shotno);

    libtokamap::TypedDataArray device = client.get_value(TestConfig::category, "DEVICE");

    const char* device_str = device.data<char>();
    assert(std::string(device_str) == "JT60SA");
}

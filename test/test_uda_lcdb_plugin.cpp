#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <plugins/pluginStructs.h>
#include <string>

#include "clientserver/errorLog.h"
#include "clientserver/udaTypes.h"
#include "clientserver/udaStructs.h"
#include "plugins/pluginStructs.h"

#include "plugin/uda_lcdb_plugin.hpp"
#include "test_plugin_helper.hpp"
#include "test_config.hpp"
#include "test_debug.hpp"

void test_uda_lcdb_plugin_help()
{
    initUdaErrorStack();
    IDAM_PLUGIN_INTERFACE plugin_interface{};
    REQUEST_DATA request_data{};
    DATA_BLOCK data_block{};

    std::strcpy(request_data.function, "help");

    plugin_interface.interfaceVersion = 1;
    plugin_interface.request_data = &request_data;
    plugin_interface.data_block = &data_block;

    int rc = udaLcdbPlugin(&plugin_interface);

    assert(rc == 0);
}

void test_uda_lcdb_plugin_get_int()
{
    initUdaErrorStack();
    int rc = 1;

    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/shotno"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_INT)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    TEST_DEBUG("test call udaLcdbPlugin method= " << interface.request_data->function);

    rc = udaLcdbPlugin(&interface);

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    assert(interface.data_block->data_type == 3);
    assert(interface.data_block->rank == 0);
    assert(interface.data_block->data_n == 1);

    int shotno = *reinterpret_cast<int*>(interface.data_block->data);

    TEST_DEBUG("type=" << interface.data_block->data_type << " rank=" << interface.data_block->rank
                       << " size=" << interface.data_block->data_n << " shotno=" << shotno);

    assert(shotno == 75000);

    destroy_plugin_interface(interface);
}

void test_uda_lcdb_plugin_get_string()
{
    initUdaErrorStack();

    int rc = 1;

    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/device"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    rc = udaLcdbPlugin(&interface);

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    printDataBlock(interface.data_block);

    // DEVICE JT60SA
    assert(interface.data_block->data_type == 17);
    assert(interface.data_block->rank == 1);
    assert(interface.data_block->data_n == 6);
    assert(interface.data_block->dims[0].dim_n == 6);

    auto t2 = reinterpret_cast<char*>(interface.data_block->data);

    assert(std::string(t2) == "JT60SA");

    destroy_plugin_interface(interface);
}

void test_uda_lcdb_plugin_get_double()
{
    initUdaErrorStack();
    int rc = 1;

    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/t2"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_DOUBLE)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    TEST_DEBUG("test call udaLcdbPlugin method= " << interface.request_data->function);

    rc = udaLcdbPlugin(&interface);

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    assert(interface.data_block->data_type == 7);
    assert(interface.data_block->rank == 0);
    assert(interface.data_block->data_n == 1);

    double t2 = *reinterpret_cast<double*>(interface.data_block->data);

    TEST_DEBUG("type=" << interface.data_block->data_type << " rank=" << interface.data_block->rank
                       << " size=" << interface.data_block->data_n << " t1=" << t2);

    assert(t2 == 1.0);

    destroy_plugin_interface(interface);
}

void test_uda_lcdb_plugin_get_array_int()
{
    initUdaErrorStack();

    int rc = 1;

    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/nexpdata"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_INT)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    rc = udaLcdbPlugin(&interface);

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    printDataBlock(interface.data_block);

    // NEXPDATA(:,1)=14, 2, 14
    assert(interface.data_block->data_type == 3);
    assert(interface.data_block->rank == 2);
    assert(interface.data_block->data_n == 3);
    assert(interface.data_block->dims[0].dim_n == 3);
    assert(interface.data_block->dims[1].dim_n == 1);

    auto values = reinterpret_cast<int*>(interface.data_block->data);

    for (int i = 0; i < interface.data_block->data_n; i++)
    {
        TEST_DEBUG("[" << i << "] = " << values[i]);
    }

    assert(values[0] == 14);
    assert(values[1] == 2);
    assert(values[2] == 14);

    destroy_plugin_interface(interface);
}

void test_uda_lcdb_plugin_get_array_double()
{
    initUdaErrorStack();

    int rc = 1;

    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/xexpdata"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_DOUBLE)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    rc = udaLcdbPlugin(&interface);

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    printDataBlock(interface.data_block);

    // XEXPDATA(:,1,1) 14 data
    // XEXPDATA(:,2,1) 14 data
    // XEXPDATA(:,3,1) 14 data
    assert(interface.data_block->data_type == 7);
    assert(interface.data_block->rank == 3);
    assert(interface.data_block->data_n == 42);
    assert(interface.data_block->dims[0].dim_n == 14);
    assert(interface.data_block->dims[1].dim_n == 3);
    assert(interface.data_block->dims[2].dim_n == 1);

    auto values = reinterpret_cast<double*>(interface.data_block->data);

    assert(values[0] == 7.42544003597721e-01);
    assert(values[16] == 0.0);
    assert(values[41] == 8.74473929358869e-01);

    destroy_plugin_interface(interface);
}

void test_uda_lcdb_plugin_get_array_string()
{
    initUdaErrorStack();

    int rc = 1;

    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/exdb"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    rc = udaLcdbPlugin(&interface);

    if (rc != 0)
    {
        concatUdaError(&interface.error_stack);

        assert(interface.error_stack.nerrors == 1);
        TEST_DEBUG("nerrors=" << interface.error_stack.nerrors);

        for (int i = 0; i < interface.error_stack.nerrors; i++)
        {
            const UDA_ERROR& err = interface.error_stack.idamerror[i];
            TEST_DEBUG("type=" << err.type << " code=" << err.code << " location=" << err.location
                               << " msg=" << err.msg);

            assert(err.code == 999);
            assert(err.type == 2);
        }
    }

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    printDataBlock(interface.data_block);

    // EXDB(:) 'YAGTS/necore'
    assert(interface.data_block->data_type == 17);
    assert(interface.data_block->rank == 2);
    assert(interface.data_block->data_n == 13);
    assert(interface.data_block->dims[0].dim_n == 1);
    assert(interface.data_block->dims[1].dim_n == 13);

    auto values = reinterpret_cast<char*>(interface.data_block->data);

    std::string value(&values[0]);

    TEST_DEBUG("value is " << value);

    assert(value == "YAGTS/necore");

    destroy_plugin_interface(interface);
}

void test_uda_lcdb_plugin_get_required_error()
{
    initUdaErrorStack();
    int rc = 0;

    // datatype is required.
    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::mapping},
                                                       {"path", "lcdb/shotno"},
                                                       {"rank", std::to_string(0)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno)},
                                                       {"category", TestConfig::category}});

    rc = udaLcdbPlugin(&interface);

    if (rc != 0)
    {
        concatUdaError(&interface.error_stack);

        assert(interface.error_stack.nerrors == 1);
        TEST_DEBUG("nerrors=" << interface.error_stack.nerrors);

        for (int i = 0; i < interface.error_stack.nerrors; i++)
        {
            const UDA_ERROR& err = interface.error_stack.idamerror[i];
            TEST_DEBUG("type=" << err.type << " code=" << err.code << " location=" << err.location
                               << " msg=" << err.msg);

            assert(err.code == 999);
            assert(err.type == 2);
        }
    }

    freeIdamErrorStack(&interface.error_stack);

    destroy_plugin_interface(interface);

    TEST_DEBUG("rc: " + std::to_string(rc));
    assert(rc == 999);
}

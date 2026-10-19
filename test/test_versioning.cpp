
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

void test_dd_version_partition()
{
    // exact selector:
    // Select the partition that matches string. 

    initUdaErrorStack();
    int rc = 1;
 
    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::versioning_mapping},
                                                       {"path", "partition-test/dd_version"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(75000)},
                                                       {"category", TestConfig::category},
                                                       {"dd_version", "3.42.2"}});

    rc = udaLcdbPlugin(&interface);

    if (rc != 0)
    {
        printUdaError(interface);
    }

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    auto data = reinterpret_cast<char*>(interface.data_block->data);

    assert(std::string(data) == "3.42.2");

    destroy_plugin_interface(interface);


    // Boundary value check: Above
    interface = generate_plugin_interface("get", {{"mapping", TestConfig::versioning_mapping},
                                                  {"path", "partition-test/dd_version"},
                                                  {"rank", std::to_string(0)},
                                                  {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                  {"root", TestConfig::root},
                                                  {"shot", std::to_string(75000)},
                                                  {"category", TestConfig::category},
                                                  {"dd_version", "4.1.1"}});

    rc = udaLcdbPlugin(&interface);

    assert(rc == 0);

    data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    data = reinterpret_cast<char*>(interface.data_block->data);

    assert(std::string(data) == "4.1.1");

    destroy_plugin_interface(interface);
}

void test_shotno_partition()
{
    // max_below selector :
    // Selects the partition whose directory name corresponds to the largest value 
    // that is strictly less than the supplied attribute value.
    //
    // partition-test/
    // ├── 0/
    // ├── 50000/
    // └── 100000/
    //
    // Attribute  ValueSelected Directory
    // 1000       0
    // 50000      0
    // 50001      50000
    // 150000     100000

    initUdaErrorStack();
    int rc = 1;

    // Boundary shotno check: less than or eaqual to
    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::versioning_mapping},
                                                       {"path", "partition-test/device"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(TestConfig::shotno_legacy_max)},
                                                       {"category", TestConfig::category},
                                                       {"dd_version", "3.42.2"}});

    rc = udaLcdbPlugin(&interface);

    if (rc != 0)
    {
        printUdaError(interface);
    }

    assert(rc == 0);

    DATA_BLOCK* data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    auto data = reinterpret_cast<char*>(interface.data_block->data);

    assert(std::string(data) == "DEVICE_LEGACY");

    destroy_plugin_interface(interface);


    // Boundary value check: greator than
    interface = generate_plugin_interface("get", {{"mapping", TestConfig::versioning_mapping},
                                                  {"path", "partition-test/device"},
                                                  {"rank", std::to_string(0)},
                                                  {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                  {"root", TestConfig::root},
                                                  {"shot", std::to_string(TestConfig::shotno_new_min)},
                                                  {"category", TestConfig::category},
                                                  {"dd_version", "3.42.2"}});

    rc = udaLcdbPlugin(&interface);
    
    if (rc != 0)
    {
        printUdaError(interface);
    }

    assert(rc == 0);

    data_block = interface.data_block;

    assert(data_block != nullptr);
    assert(data_block->data != nullptr);

    data = reinterpret_cast<char*>(interface.data_block->data);

    assert(std::string(data) == "DEVICE_NEW");

    destroy_plugin_interface(interface);
}

void test_dd_version_partition_not_found()
{
    initUdaErrorStack();
    int rc = 1;
 
    auto interface = generate_plugin_interface("get", {{"mapping", TestConfig::versioning_mapping},
                                                       {"path", "partition-test/dd_version"},
                                                       {"rank", std::to_string(0)},
                                                       {"datatype", std::to_string(UDA_TYPE_STRING)},
                                                       {"root", TestConfig::root},
                                                       {"shot", std::to_string(75000)},
                                                       {"category", TestConfig::category},
                                                       {"dd_version", "9.9.9"}});

    rc = udaLcdbPlugin(&interface);

    if (rc != 0)
    {
        printUdaError(interface);
    }

    assert(rc != 0);

}

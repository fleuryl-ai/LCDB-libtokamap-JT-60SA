#include <cassert>
#include <iostream>

#include <libtokamap/libtokamap.hpp>

#include "datasource/lcdb_data_source.hpp"
#include "test_config.hpp"
#include "test_debug.hpp"
#include <filesystem>

#ifndef LCDB_DEFAULT_CONFIG
#define LCDB_DEFAULT_CONFIG ""
#endif

void test_mapping_handler()
{
    TEST_DEBUG("cwd=" << std::filesystem::current_path());
    const char* config = std::getenv("LCDB_CONFIG");

    TEST_DEBUG("config = " << (config ? config : "(null)"));

    TEST_DEBUG("LCDB_DEFAULT_CONFIG:" << LCDB_DEFAULT_CONFIG);

    if (config == nullptr || *config == '\0')
    {
        config = LCDB_DEFAULT_CONFIG;
    }

    libtokamap::MappingHandler handler;

    handler.init(std::filesystem::path(config));

    TEST_DEBUG("after init");
    try
    {
        handler.register_data_source("LCDB",
                                     std::make_unique<lcdb::LcdbDataSource>(TestConfig::root, TestConfig::shotno));
        auto result =
            handler.map("test", "lcdb/xexpdata", libtokamap::DataType::Double, 3, {{"category", "test.s001"}});

        TEST_DEBUG("rank = " << result.rank());
        TEST_DEBUG("size = " << result.size());

        auto xexpdata_array = result.to_vector<double>();
        auto xexpdata_shape = result.shape();

        // shape
        assert(xexpdata_shape[0] == 14);
        assert(xexpdata_shape[1] == 3);
        assert(xexpdata_shape[2] == 1);
        // data
        assert(xexpdata_array[0] == 7.42544003597721e-01);
        assert(xexpdata_array[3] == 7.17403463764677e-01);
    }
    catch (const std::exception& e)
    {
        std::cerr << "type=" << typeid(e).name() << "\nwhat=" << e.what() << std::endl;
        throw;
    }
}

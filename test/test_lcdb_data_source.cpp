#include "exceptions/exceptions.hpp"
#include "datasource/lcdb_data_source.hpp"
#include "map_types/data_source_mapping.hpp"
#include "map_types/map_arguments.hpp"
#include "test_config.hpp"
#include "test_debug.hpp"
#include "libtokamap/libtokamap.hpp"

void test_lcdb_data_source_get()
{
    lcdb::LcdbDataSource datasource(TestConfig::root, TestConfig::shotno);

    libtokamap::DataSourceArgs args;

    args["category"] = TestConfig::category;
    args["name"] = "XEXPDATA";

    std::unordered_map<std::string, std::unique_ptr<libtokamap::Mapping>> entries;

    nlohmann::json global_data = {};

    libtokamap::MapArguments map_args(entries, global_data, libtokamap::DataType::Double, 1, false, false, nullptr);

    auto result = datasource.get(args, map_args, nullptr);

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

void test_lcdb_data_source_nocategory()
{
    bool caught = false;

    lcdb::LcdbDataSource datasource(TestConfig::root, TestConfig::shotno);

    libtokamap::DataSourceArgs args;

    args["name"] = "XEXPDATA";

    std::unordered_map<std::string, std::unique_ptr<libtokamap::Mapping>> entries;

    nlohmann::json global_data = {};

    libtokamap::MapArguments map_args(entries, global_data, libtokamap::DataType::Double, 1, false, false, nullptr);

    try
    {
        auto result = datasource.get(args, map_args, nullptr);
    }
    catch (libtokamap::DataSourceError&)
    {
        caught = true;
    }

    assert(caught == true);
}

void test_lcdb_data_source_noname()
{
    bool caught = false;

    lcdb::LcdbDataSource datasource(TestConfig::root, TestConfig::shotno);

    libtokamap::DataSourceArgs args;

    args["category"] = TestConfig::category;

    std::unordered_map<std::string, std::unique_ptr<libtokamap::Mapping>> entries;

    nlohmann::json global_data = {};

    libtokamap::MapArguments map_args(entries, global_data, libtokamap::DataType::Double, 1, false, false, nullptr);

    try
    {
        auto result = datasource.get(args, map_args, nullptr);
    }
    catch (libtokamap::DataSourceError&)
    {
        caught = true;
    }

    assert(caught == true);
}

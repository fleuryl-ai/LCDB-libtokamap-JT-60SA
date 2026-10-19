#include <iostream>
#include <exception>
#include <vector>

// 各テスト関数
void test_load();
void test_open();
void test_close();
void test_lcdb_open_close();
void test_lcdb_duplicate_open();
void test_lcdb_close_unopened();
void test_lcdb_close_all();
void test_lcdb_get_value();
void test_lcdb_get_value_string_array();
void test_lcdb_get_value_unopend();
void test_lcdb_data_source_get();
void test_lcdb_data_source_nocategory();
void test_lcdb_data_source_noname();
void test_mapping_handler();
void test_uda_lcdb_plugin_help();
void test_uda_lcdb_plugin_get_int();
void test_uda_lcdb_plugin_get_string();
void test_uda_lcdb_plugin_get_double();
void test_uda_lcdb_plugin_get_array_int();
void test_uda_lcdb_plugin_get_array_double();
void test_uda_lcdb_plugin_get_array_string();
void test_uda_lcdb_plugin_get_required_error();
void test_shotno_partition();
void test_dd_version_partition();
void test_dd_version_partition_not_found();

struct TestCase
{
    const char* name;
    void (*func)();
};

int main()
{
    const std::vector<TestCase> tests = {
        {"test_load", test_load},
        {"test_open", test_open},
        {"test_close", test_close},
        {"test_lcdb_open_close", test_lcdb_open_close},
        {"test_lcdb_duplicate_open", test_lcdb_duplicate_open},
        {"test_lcdb_close_unopened", test_lcdb_close_unopened},
        {"test_lcdb_close_all", test_lcdb_close_all},
        {"test_lcdb_get_value", test_lcdb_get_value},
        {"test_lcdb_get_value_string_array", test_lcdb_get_value_string_array},
        {"test_lcdb_get_value_unopend", test_lcdb_get_value_unopend},
        {"test_lcdb_data_source_get", test_lcdb_data_source_get},
        {"test_lcdb_data_source_nocategory", test_lcdb_data_source_nocategory},
        {"test_lcdb_data_source_noname", test_lcdb_data_source_noname},
        {"test_mapping_handler", test_mapping_handler},
        {"test_uda_lcdb_plugin_help", test_uda_lcdb_plugin_help},
        {"test_uda_lcdb_plugin_get_int", test_uda_lcdb_plugin_get_int},
        {"test_uda_lcdb_plugin_get_string", test_uda_lcdb_plugin_get_string},
        {"test_uda_lcdb_plugin_get_double", test_uda_lcdb_plugin_get_double},
        {"test_uda_lcdb_plugin_get_array_int", test_uda_lcdb_plugin_get_array_int},
        {"test_uda_lcdb_plugin_get_array_double", test_uda_lcdb_plugin_get_array_double},
        {"test_uda_lcdb_plugin_get_array_string", test_uda_lcdb_plugin_get_array_string},
        {"test_uda_lcdb_plugin_get_required_error", test_uda_lcdb_plugin_get_required_error},
        {"test_shotno_partition", test_shotno_partition},
        {"test_dd_version_partition", test_dd_version_partition},
        {"test_dd_version_partition_not_found", test_dd_version_partition_not_found},
    };

    std::cout << "----- test liblcdb.so -----" << std::endl;

    for (const auto& test : tests)
    {
        try
        {
            std::cout << "[RUN ] " << test.name << std::endl;

            test.func();

            std::cout << "[ OK ] " << test.name << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "[FAIL] " << test.name << ": " << e.what() << std::endl;

            return 1;
        }
    }

    std::cout << "\nAll tests passed." << std::endl;

    return 0;
}

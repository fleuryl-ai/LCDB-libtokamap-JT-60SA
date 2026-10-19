// test_config.hpp

#pragma once

struct TestConfig
{
    static constexpr const char* root = "~/work/project/libtokamap_cpp/test/data";

    static constexpr int shotno = 75000;

    static constexpr const char* category = "test.s001";

    static constexpr const char* category2 = "test.s002";

    static constexpr const char* mapping = "test";

    static constexpr int shotno_legacy_max = 50000;

    static constexpr int shotno_new_min = 50001;

    static constexpr const char* versioning_mapping = "versioning";
};

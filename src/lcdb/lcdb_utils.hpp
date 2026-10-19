// lcdb_utils.hpp

#pragma once

#include <stdexcept>
#include <string>

inline void check_rc(const std::string& func, int rc)
{
    if (rc != 0)
    {
        throw std::runtime_error(func + " failed: " + std::to_string(rc));
    }
}

#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace lcdb
{

enum class DataType
{
    Int = 1,
    Double = 2,
    String = 3
};

struct ValueInfo
{
    DataType datatype;
    int dimension;
    std::vector<int> shape;
    int maxlen;
};

template <typename T> struct NdArray
{
    std::vector<int> shape;
    std::vector<T> data;

    bool valid() const
    {
        std::size_t expected = 1;

        for (int dim : shape)
        {
            expected *= dim;
        }

        return expected == data.size();
    }

    void print() const
    {
        std::cout << "shape = [";

        for (std::size_t i = 0; i < shape.size(); ++i)
        {
            if (i != 0)
            {
                std::cout << ", ";
            }

            std::cout << shape[i];
        }

        std::cout << "]\n";

        for (const auto& v : data)
        {
            std::cout << v << " ";
        }

        std::cout << '\n';
    }
};

using IntArray = NdArray<int>;

using DoubleArray = NdArray<double>;

using StringArray = NdArray<std::string>;

using Value = std::variant<int, double, std::string, IntArray, DoubleArray, StringArray>;

} // namespace lcdb

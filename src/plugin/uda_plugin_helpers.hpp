#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <uda/clientserver/compressDim.h>
#include <uda/clientserver/initStructs.h>
#include <uda/clientserver/udaTypes.h>

#include "utils/typed_data_array.hpp"

namespace uda_lcdb
{

inline UDA_TYPE uda_type_of(libtokamap::DataType type)
{
    using libtokamap::DataType;

    switch (type)
    {
    case DataType::Int32:
        return UDA_TYPE_INT;

    case DataType::Double:
        return UDA_TYPE_DOUBLE;

    case DataType::Int8:
        return UDA_TYPE_STRING;

    default:
        throw std::runtime_error("Unsupported TypedDataArray type");
    }
}

template <typename T> int set_return_scalar(DATA_BLOCK* data_block, T value)
{
    initDataBlock(data_block);

    auto* data = static_cast<T*>(std::malloc(sizeof(T)));

    *data = value;

    data_block->rank = 0;
    data_block->dims = nullptr;
    data_block->data_n = 1;

    data_block->data_type = uda_type_of(libtokamap::data_type_of<T>());

    data_block->data = reinterpret_cast<char*>(data);

    return 0;
}

template <typename T>
int set_return_array(DATA_BLOCK* data_block, const T* values, size_t size, const std::vector<size_t>& shape)
{
    initDataBlock(data_block);

    data_block->rank = static_cast<int>(shape.size());

    data_block->dims = static_cast<DIMS*>(std::malloc(sizeof(DIMS) * shape.size()));

    for (size_t i = 0; i < shape.size(); ++i)
    {
        initDimBlock(&data_block->dims[i]);

        data_block->dims[i].data_type = UDA_TYPE_UNSIGNED_INT;

        data_block->dims[i].dim_n = static_cast<int>(shape[i]);

        data_block->dims[i].compressed = 1;
        data_block->dims[i].dim0 = 0.0;
        data_block->dims[i].diff = 1.0;
        data_block->dims[i].method = 0;
    }

    auto* data = static_cast<T*>(std::malloc(sizeof(T) * size));

    std::copy(values, values + size, data);

    data_block->data = reinterpret_cast<char*>(data);

    data_block->data_n = static_cast<int>(size);

    data_block->data_type = uda_type_of(libtokamap::data_type_of<T>());

    return 0;
}

inline int typed_data_array_to_datablock(DATA_BLOCK* data_block, const libtokamap::TypedDataArray& array,
                                         UDA_TYPE datatype)
{
    switch (datatype)
    {
    case UDA_TYPE_DOUBLE:
    {
        auto* data = reinterpret_cast<const double*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_FLOAT:
    {
        auto* data = reinterpret_cast<const float*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_SHORT:
    {
        auto* data = reinterpret_cast<const int16_t*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_INT:
    {
        auto* data = reinterpret_cast<const int32_t*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_LONG:
    {
        auto* data = reinterpret_cast<const long*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_LONG64:
    {
        auto* data = reinterpret_cast<const int64_t*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_UNSIGNED_SHORT:
    {
        auto* data = reinterpret_cast<const uint16_t*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_UNSIGNED_INT:
    {
        auto* data = reinterpret_cast<const uint32_t*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_UNSIGNED_LONG:
    {
        auto* data = reinterpret_cast<const unsigned long*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_UNSIGNED_LONG64:
    {
        auto* data = reinterpret_cast<const uint64_t*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    case UDA_TYPE_CHAR:
    case UDA_TYPE_STRING:
    {
        auto* data = reinterpret_cast<const char*>(array.buffer());

        if (array.rank() == 0)
        {
            return set_return_scalar(data_block, data[0]);
        }

        return set_return_array(data_block, data, array.size(), array.shape());
    }

    default:
        throw std::runtime_error("Unsupported UDA datatype");
    }
}

} // namespace uda_lcdb

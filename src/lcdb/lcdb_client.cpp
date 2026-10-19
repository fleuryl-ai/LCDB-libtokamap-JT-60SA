#include "lcdb_client.hpp"
#include "lcdb_c_api.hpp"
#include "utils/typed_data_array.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>

namespace lcdb
{

LcdbClient::LcdbClient(const std::string& root, int shotno) : root_(root), shotno_(shotno) {}

LcdbClient::~LcdbClient()
{
    try
    {
        close_all();
    }
    catch (...)
    {
    }
}

//
// Public API
//

void LcdbClient::setup(const std::string& root, int shotno)
{
    root_ = root;
    shotno_ = shotno;
}

void LcdbClient::open_session(const std::string& category)
{
    if (sessions_.find(category) != sessions_.end())
    {
        throw std::runtime_error("Already opened: " + category);
    }

    if (sessions_.size() >= 10)
    {
        throw std::runtime_error("Max sessions exceeded");
    }

    int rc = lcdbOpen(root_.c_str(), shotno_, category.c_str());

    if (rc != 0)
    {
        throw std::runtime_error("lcdbOpen failed: " + std::to_string(rc));
    }

    sessions_.emplace(category, Session{root_, shotno_, category});
}

void LcdbClient::close_session(const std::string& category)
{
    auto it = sessions_.find(category);

    if (it == sessions_.end())
    {
        throw std::runtime_error("Session not opened: " + category);
    }

    int rc = lcdbClose(it->second.root.c_str(), it->second.shotno, it->second.category.c_str());

    if (rc != 0)
    {
        throw std::runtime_error("lcdbClose failed: " + std::to_string(rc));
    }

    sessions_.erase(it);
}

void LcdbClient::close_all()
{
    for (auto it = sessions_.begin(); it != sessions_.end();)
    {
        int rc = lcdbClose(it->second.root.c_str(), it->second.shotno, it->second.category.c_str());

        if (rc != 0)
        {
            throw std::runtime_error("lcdbClose failed: " + std::to_string(rc));
        }

        it = sessions_.erase(it);
    }
}

libtokamap::TypedDataArray LcdbClient::get_value(const std::string& category, const std::string& dataname)
{
    Session& session = get_session(category);

    ValueInfo info = get_value_info(session, category, dataname);

    //
    // array
    //
    if (info.dimension != 0)
    {
        const std::size_t size = calc_size(info.shape);

        switch (info.datatype)
        {
        case lcdb::DataType::Int:
        {
            std::vector<int> values(size);

            int rc = lcdbValue(session.root.c_str(), session.shotno, session.category.c_str(), dataname.c_str(),
                               static_cast<int>(info.datatype), info.dimension, info.shape.data(), 0, values.data());

            if (rc != 0)
            {
                throw std::runtime_error("lcdbValue failed:" + std::to_string(rc) + "category: " + category +
                                         "dataname: " + dataname);
            }

            std::vector<size_t> shape(info.shape.begin(), info.shape.end());

            return libtokamap::TypedDataArray(values, shape);
        }

        case lcdb::DataType::Double:
        {
            std::vector<double> values(size);

            int rc = lcdbValue(session.root.c_str(), session.shotno, session.category.c_str(), dataname.c_str(),
                               static_cast<int>(info.datatype), info.dimension, info.shape.data(), 0, values.data());

            if (rc != 0)
            {
                throw std::runtime_error("lcdbValue failed:" + std::to_string(rc) + "category: " + category +
                                         "dataname: " + dataname);
            }

            std::vector<size_t> shape(info.shape.begin(), info.shape.end());

            return libtokamap::TypedDataArray(values, shape);
        }

        case lcdb::DataType::String:
        {
            // Stringの配列はInt8の2次元配列として扱う
            const std::size_t count = calc_size(info.shape);

            std::vector<char> buffer(count * (info.maxlen + 1), '\0');

            int rc = lcdbValue(session.root.c_str(), session.shotno, session.category.c_str(), dataname.c_str(),
                               static_cast<int>(info.datatype), info.dimension, info.shape.data(), info.maxlen,
                               buffer.data());

            if (rc != 0)
            {
                throw std::runtime_error("lcdbValue failed:" + std::to_string(rc) + "category: " + category +
                                         "dataname: " + dataname);
            }

            std::vector<size_t> shape{count, static_cast<size_t>(info.maxlen + 1)};

            return libtokamap::TypedDataArray(buffer.data(), buffer.size(), shape);
        }
        }
    }

    //
    // scalar
    //
    std::vector<int> shape = info.shape;

    switch (info.datatype)
    {
    case DataType::Int:
    {
        int value{};

        int rc = lcdbValue(session.root.c_str(), session.shotno, session.category.c_str(), dataname.c_str(),
                           static_cast<int>(info.datatype), info.dimension, shape.empty() ? nullptr : shape.data(), 0,
                           &value);

        if (rc != 0)
        {
            throw std::runtime_error("lcdbValue failed:" + std::to_string(rc) + "category: " + category +
                                     "dataname: " + dataname);
        }

        return libtokamap::TypedDataArray(value);
    }

    case DataType::Double:
    {
        double value{};

        int rc = lcdbValue(session.root.c_str(), session.shotno, session.category.c_str(), dataname.c_str(),
                           static_cast<int>(info.datatype), info.dimension, shape.empty() ? nullptr : shape.data(), 0,
                           &value);

        if (rc != 0)
        {
            throw std::runtime_error("lcdbValue failed:" + std::to_string(rc) + "category: " + category +
                                     "dataname: " + dataname);
        }

        return libtokamap::TypedDataArray(value);
    }

    case DataType::String:
    {
        std::vector<char> value(info.maxlen + 1, '\0');

        int rc = lcdbValue(session.root.c_str(), session.shotno, session.category.c_str(), dataname.c_str(),
                           static_cast<int>(info.datatype), info.dimension, shape.empty() ? nullptr : shape.data(),
                           info.maxlen, value.data());

        if (rc != 0)
        {
            throw std::runtime_error("lcdbValue failed:" + std::to_string(rc) + "category: " + category +
                                     "dataname: " + dataname);
        }

        return libtokamap::TypedDataArray(std::string(value.data()));
    }
    }

    throw std::runtime_error("unsupported datatype");
}

//
// Private method
//

Session& LcdbClient::get_session(const std::string& category)
{
    open_session_if_needed(category);

    auto it = sessions_.find(category);

    if (it == sessions_.end())
    {
        throw std::runtime_error("Session not opened: " + category);
    }

    return it->second;
}

void LcdbClient::open_session_if_needed(const std::string& category)
{
    if (sessions_.find(category) != sessions_.end())
    {
        return;
    }

    open_session(category);
}

lcdb::ValueInfo LcdbClient::get_value_info(const Session& session, const std::string& category, const std::string& name)
{
    int datatype = 0;
    int dimension = 0;

    int rc = lcdbValueDim(session.root.c_str(), session.shotno, session.category.c_str(), name.c_str(), &dimension);

    if (rc != 0)
    {
        throw std::runtime_error("lcdbValueDim failed: " + std::to_string(rc));
    }

    std::vector<int> shape;
    int maxlen = 0;

    if (dimension > 0)
    {
        shape.resize(dimension);
    }

    rc = lcdbValueType(session.root.c_str(), session.shotno, session.category.c_str(), name.c_str(), &datatype,
                       &dimension, shape.empty() ? nullptr : shape.data(), &maxlen);

    if (rc != 0)
    {
        throw std::runtime_error("ldbcValueType" + std::to_string(rc));
    }

    ValueInfo info;
    info.datatype = static_cast<lcdb::DataType>(datatype);
    info.dimension = dimension;
    info.maxlen = maxlen;
    info.shape = shape;

    return info;
}

std::size_t LcdbClient::calc_size(const std::vector<int>& shape)
{
    std::size_t size = 1;

    for (int dim : shape)
    {
        size *= dim;
    }

    return size;
}
} // namespace lcdb

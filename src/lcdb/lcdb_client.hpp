#pragma once

#include "lcdb_types.hpp"
#include "lcdb_session.hpp"
#include "libtokamap/libtokamap.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace lcdb
{

class LcdbClient
{
  public:
    explicit LcdbClient(const std::string& root = "", int shotno = 0);

    ~LcdbClient();

    void setup(const std::string& root, int shotno);

    void open_session(const std::string& category);

    void close_session(const std::string& category);

    void close_all();

    libtokamap::TypedDataArray get_value(const std::string& category, const std::string& dataname);

  private:
    Session& get_session(const std::string& category);

    void open_session_if_needed(const std::string& category);

    std::vector<std::string> list_dnames(const std::string& category);

    ValueInfo get_value_info(const Session& session, const std::string& category, const std::string& name);

    std::size_t calc_size(const std::vector<int>& shape);

  private:
    std::string root_;
    int shotno_{};

    std::unordered_map<std::string, Session> sessions_;
};

} // namespace lcdb

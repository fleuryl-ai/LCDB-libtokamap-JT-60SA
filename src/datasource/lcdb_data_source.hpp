#pragma once

#include <libtokamap/libtokamap.hpp>

#include "lcdb/lcdb_client.hpp"

namespace lcdb
{

class LcdbDataSource : public libtokamap::DataSource
{
  public:
    explicit LcdbDataSource(const std::string& root, int shotno);

    ~LcdbDataSource() override = default;

    libtokamap::TypedDataArray get(const libtokamap::DataSourceArgs& map_args,
                                   const libtokamap::MapArguments& arguments, libtokamap::RamCache* ram_cache) override;

  private:
    LcdbClient client_;
};

} // namespace lcdb

#include "datasource/lcdb_data_source.hpp"
#include "lcdb/lcdb_client.hpp"

namespace lcdb
{

LcdbDataSource::LcdbDataSource(const std::string& root, int shotno) : client_(root, shotno) {}

libtokamap::TypedDataArray LcdbDataSource::get(const libtokamap::DataSourceArgs& map_args,
                                               const libtokamap::MapArguments& arguments,
                                               libtokamap::RamCache* ram_cache)
{
    try
    {
        (void)arguments;
        (void)ram_cache;

        if (!map_args.contains("category"))
        {
            throw libtokamap::DataSourceError("Missing 'category' argument");
        }

        if (!map_args.contains("name"))
        {
            throw libtokamap::DataSourceError("Missing 'name' argument");
        }

        const std::string category = map_args.at("category").get<std::string>();

        const std::string name = map_args.at("name").get<std::string>();

        return client_.get_value(category, name);
    }
    catch (const libtokamap::DataSourceError&)
    {
        // Re-throw LibTokaMap errors
        throw;
    }
    catch (const std::exception& e)
    {
        throw libtokamap::DataSourceError("Failed to retrieve data: " + std::string(e.what()));
    }
}

} // namespace lcdb

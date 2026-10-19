#include "uda_lcdb_plugin.hpp"

#include <typeindex>
#include <cstdlib>
#include <filesystem>
#include <memory>

#include <clientserver/initStructs.h>
#include <clientserver/stringUtils.h>
#include <plugins/udaPlugin.h>

#include <libtokamap/libtokamap.hpp>

#include "datasource/lcdb_data_source.hpp"
#include "plugin/uda_plugin_helpers.hpp"

#ifndef LCDB_DEFAULT_CONFIG
#define LCDB_DEFAULT_CONFIG ""
#endif

namespace
{

constexpr const char* kPluginVersion = "1";
constexpr int kMaxInterfaceVersion = 1;
constexpr const char* kDefaultMethod = "read";

} // namespace

int udaLcdbPlugin(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    static UdaLcdbPlugin plugin;

    if (plugin_interface->interfaceVersion > kMaxInterfaceVersion)
    {
        RAISE_PLUGIN_ERROR("Plugin Interface Version Unknown");
    }

    plugin_interface->pluginVersion = std::strtol(kPluginVersion, nullptr, 10);

    REQUEST_DATA* request_data = plugin_interface->request_data;

    if (plugin_interface->housekeeping || STR_IEQUALS(request_data->function, "reset"))
    {
        plugin.reset();
        return 0;
    }

    plugin.init();

    if (STR_IEQUALS(request_data->function, "help"))
    {
        return plugin.help(plugin_interface);
    }
    else if (STR_IEQUALS(request_data->function, "version"))
    {
        return plugin.version(plugin_interface);
    }
    else if (STR_IEQUALS(request_data->function, "builddate"))
    {
        return plugin.build_date(plugin_interface);
    }
    else if (STR_IEQUALS(request_data->function, "defaultmethod"))
    {
        return plugin.default_method(plugin_interface);
    }
    else if (STR_IEQUALS(request_data->function, "maxinterfaceversion"))
    {
        return plugin.max_interface_version(plugin_interface);
    }
    else if (STR_IEQUALS(request_data->function, "get"))
    {
        return plugin.get(plugin_interface);
    }

    RAISE_PLUGIN_ERROR("Unknown function requested");
}

void UdaLcdbPlugin::init()
{
    if (!initialized_)
    {
    }

    initialized_ = true;
}

void UdaLcdbPlugin::reset()
{
    initialized_ = false;
}

int UdaLcdbPlugin::help(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    return setReturnDataString(plugin_interface->data_block, "LCDB Mapping Plugin", "LCDB Mapping Plugin");
}

int UdaLcdbPlugin::version(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    return setReturnDataString(plugin_interface->data_block, kPluginVersion, "Plugin version");
}

int UdaLcdbPlugin::build_date(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    return setReturnDataString(plugin_interface->data_block, __DATE__, "Build date");
}

int UdaLcdbPlugin::default_method(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    return setReturnDataString(plugin_interface->data_block, kDefaultMethod, "Default method");
}

int UdaLcdbPlugin::max_interface_version(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    return setReturnDataIntScalar(plugin_interface->data_block, kMaxInterfaceVersion, "Maximum Interface Version");
}

int UdaLcdbPlugin::get(IDAM_PLUGIN_INTERFACE* plugin_interface)
{
    try
    {
        DATA_BLOCK* data_block = plugin_interface->data_block;

        initDataBlock(data_block);

        data_block->rank = 0;
        data_block->dims = nullptr;

        REQUEST_DATA* request_data = plugin_interface->request_data;

        //
        // IMAS plugin supplied arguments
        //

        const char* mapping = nullptr;

        FIND_REQUIRED_STRING_VALUE(request_data->nameValueList, mapping);

        const char* path = nullptr;

        FIND_REQUIRED_STRING_VALUE(request_data->nameValueList, path);

        int rank = 0;

        FIND_REQUIRED_INT_VALUE(request_data->nameValueList, rank);

        int datatype = 0;

        FIND_REQUIRED_INT_VALUE(request_data->nameValueList, datatype);

        //
        // Mapping-specific arguments
        //

        const char* root = nullptr;

        FIND_REQUIRED_STRING_VALUE(request_data->nameValueList, root);

        int shot = 0;

        FIND_REQUIRED_INT_VALUE(request_data->nameValueList, shot);

        const char* category = nullptr;

        FIND_REQUIRED_STRING_VALUE(request_data->nameValueList, category);

        const char* dd_version = nullptr;

        FIND_STRING_VALUE(request_data->nameValueList, dd_version);

        //
        // Convert datatype
        // UDA_TYPE_INT       → DataType::Int32
        // UDA_TYPE_DOUBLE    → DataType::Double
        // UDA_TYPE_STRING    → DataType::Int8
        // UDA_TYPE_CHAR      → DataType::Int8

        libtokamap::DataType requested_type = libtokamap::DataType::Unknown;

        switch (datatype)
        {
        case UDA_TYPE_DOUBLE:
            requested_type = libtokamap::DataType::Double;
            break;

        case UDA_TYPE_INT:
            requested_type = libtokamap::DataType::Int32;
            break;

        case UDA_TYPE_CHAR:
            requested_type = libtokamap::DataType::Int8;
            break;

        case UDA_TYPE_STRING:
            requested_type = libtokamap::DataType::Int8;
            break;

        default:
            RAISE_PLUGIN_ERROR("Unsupported datatype");
        }

        //
        // Build datasource
        //

        libtokamap::MappingHandler handler;

        const char* config = std::getenv("LCDB_CONFIG");

        if (!config || *config == '\0')
        {
            config = LCDB_DEFAULT_CONFIG;
        }

        handler.init(std::filesystem::path(config));
        handler.register_data_source("LCDB", std::make_unique<lcdb::LcdbDataSource>(root, shot));

        //
        // Mapping variables
        //

        nlohmann::json attrs;
        attrs["category"] = category;
        attrs["shot"] = shot;

        if (dd_version != nullptr)
        {
            attrs["dd_version"] = dd_version;
        }

        //
        // Execute mapping
        //

        auto result = handler.map(mapping, path, requested_type, rank, attrs);

        //
        // TypedDataArray -> DATA_BLOCK
        //

        return uda_lcdb::typed_data_array_to_datablock(data_block, result, static_cast<UDA_TYPE>(datatype));
    }
    catch (const std::exception& ex)
    {
        RAISE_PLUGIN_ERROR(ex.what());
    }
    catch (...)
    {
        RAISE_PLUGIN_ERROR("Unknown exception");
    }
}

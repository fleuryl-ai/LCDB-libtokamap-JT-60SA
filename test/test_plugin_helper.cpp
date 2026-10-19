#include "test_plugin_helper.hpp"
#include "test_debug.hpp"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <plugins/pluginStructs.h>
#include <uda/clientserver/initStructs.h>
#include <uda/structures/struct.h>
#include "clientserver/errorLog.h"

IDAM_PLUGIN_INTERFACE generate_plugin_interface(const std::string& function,
                                                const std::vector<std::pair<std::string, std::string>>& arguments)
{
    IDAM_PLUGIN_INTERFACE interface{};

    interface.dbgout = stdout;
    interface.errout = stderr;

    interface.data_block = new DATA_BLOCK;
    initDataBlock(interface.data_block);

    interface.request_data = new REQUEST_DATA;
    initRequestData(interface.request_data);

    std::strcpy(interface.request_data->function, function.c_str());

    initNameValueList(&interface.request_data->nameValueList);

    interface.request_data->nameValueList.listSize = static_cast<int>(arguments.size());

    interface.request_data->nameValueList.pairCount = static_cast<int>(arguments.size());

    interface.request_data->nameValueList.nameValue =
        static_cast<NAMEVALUE*>(std::calloc(arguments.size(), sizeof(NAMEVALUE)));

    int i = 0;

    for (const auto& pair : arguments)
    {
        NAMEVALUE* nv = &interface.request_data->nameValueList.nameValue[i];

        nv->pair = ::strdup((pair.first + "=" + pair.second).c_str());

        nv->name = ::strdup(pair.first.c_str());

        nv->value = ::strdup(pair.second.c_str());

        ++i;
    }

    interface.userdefinedtypelist = new USERDEFINEDTYPELIST;

    initUserDefinedTypeList(interface.userdefinedtypelist);

    interface.logmalloclist = new LOGMALLOCLIST;

    initLogMallocList(interface.logmalloclist);

    interface.interfaceVersion = 1;

    return interface;
}

void destroy_plugin_interface(IDAM_PLUGIN_INTERFACE& interface)
{
    if (interface.request_data != nullptr)
    {
        NAMEVALUELIST& nvl = interface.request_data->nameValueList;

        for (int i = 0; i < nvl.pairCount; ++i)
        {
            std::free(nvl.nameValue[i].pair);
            std::free(nvl.nameValue[i].name);
            std::free(nvl.nameValue[i].value);
        }

        std::free(nvl.nameValue);

        delete interface.request_data;
        interface.request_data = nullptr;
    }

    delete interface.data_block;
    interface.data_block = nullptr;

    delete interface.userdefinedtypelist;
    interface.userdefinedtypelist = nullptr;

    delete interface.logmalloclist;
    interface.logmalloclist = nullptr;
}

void printDataBlock(const DATA_BLOCK* db)
{
    TEST_DEBUG("rank    = " << db->rank << '\n' << "data_n  = " << db->data_n << '\n' << "type    = " << db->data_type);

    if (db->dims != nullptr)
    {
        for (int i = 0; i < db->rank; ++i)
        {
            TEST_DEBUG("dim[" << i << "] = " << db->dims[i].dim_n);
        }
    }
}

void printUdaError(IDAM_PLUGIN_INTERFACE& interface)
{
    concatUdaError(&interface.error_stack);

    TEST_DEBUG("nerrors=" << interface.error_stack.nerrors);

    for (int i = 0; i < interface.error_stack.nerrors; i++)
    {
        const UDA_ERROR& err = interface.error_stack.idamerror[i];
        TEST_DEBUG("type=" << err.type << " code=" << err.code << " location=" << err.location << " msg=" << err.msg);
    }
}

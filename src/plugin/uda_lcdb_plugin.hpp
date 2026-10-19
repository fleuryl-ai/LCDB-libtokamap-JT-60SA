#pragma once

#include <plugins/pluginStructs.h>

class UdaLcdbPlugin
{
  public:
    void init();
    void reset();

    int help(IDAM_PLUGIN_INTERFACE* plugin_interface);

    int version(IDAM_PLUGIN_INTERFACE* plugin_interface);

    int build_date(IDAM_PLUGIN_INTERFACE* plugin_interface);

    int default_method(IDAM_PLUGIN_INTERFACE* plugin_interface);

    int max_interface_version(IDAM_PLUGIN_INTERFACE* plugin_interface);

    int get(IDAM_PLUGIN_INTERFACE* plugin_interface);

  private:
    bool initialized_ = false;
};

extern "C" int udaLcdbPlugin(IDAM_PLUGIN_INTERFACE* plugin_interface);

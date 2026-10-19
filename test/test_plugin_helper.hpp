#pragma once

#include "clientserver/udaStructs.h"
#include <string>
#include <utility>
#include <vector>

#include <uda/clientserver/initStructs.h>
#include <uda/plugins/pluginStructs.h>

IDAM_PLUGIN_INTERFACE generate_plugin_interface(const std::string& function,
                                                const std::vector<std::pair<std::string, std::string>>& arguments);

void destroy_plugin_interface(IDAM_PLUGIN_INTERFACE& interface);

void printDataBlock(const DATA_BLOCK*);

void printUdaError(IDAM_PLUGIN_INTERFACE& interface);

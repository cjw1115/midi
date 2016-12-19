#pragma once
#include "common.h"
#include <string>


export_dll uint load_vst_plugin(const char * filename);

export_dll uint unload_vst_plugin(const char * filename);
#pragma once
#include "common.h"

export_dll bool open_output();
export_dll void close_output();
export_dll uint output_short_msg(unsigned int msg);
export_dll uint output_long_msg();
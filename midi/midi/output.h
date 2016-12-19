#pragma once
#include "common.h"

export_dll bool open_output();
export_dll void close_output();
export_dll uint output_reset();
export_dll uint output_msg(unsigned int msg);
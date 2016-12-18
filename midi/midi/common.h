#pragma once

#ifndef export_dll
#define export_dll extern "C" __declspec(dllexport)
#endif // !export_dll

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned long ulong;
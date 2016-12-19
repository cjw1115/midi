#include "output.h"
#include <iostream>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

static HMIDIOUT handle;
static bool opened = false;

bool open_output()
{
	if (opened)
		return true;
	if (midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL))
	{
		return false;
	}
	opened = true;
	return true;
}

void close_output()
{
	if (!opened)
		return;

	midiOutClose(handle);
	opened = false;
}

uint output_msg(unsigned int msg)
{
	if (!opened)
	{
		open_output();
	}
	return midiOutShortMsg(handle, msg);
}

uint output_reset()
{
	return midiOutReset(handle);
}

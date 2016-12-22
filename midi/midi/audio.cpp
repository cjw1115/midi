#include "audio.h"
#include <iostream>

midi::audio::audio()
{
	this->_valid = true;
	err = Pa_Initialize();
	if (err != paNoError)
	{
		this->print_error();
		this->_valid = false;
	}
}

midi::audio::~audio()
{
	err = Pa_Terminate();
	if (err != paNoError)
	{
		this->print_error();
	}
}

std::atomic_bool midi::audio::valid()
{
	return this->_valid;
}

const bool midi::audio::open()
{
	err = Pa_OpenDefaultStream(&stream,
		0,          /* no input channels */
		2,          /* stereo output */
		paFloat32,  /* 32 bit floating point output */
		44100,
		256,        /* frames per buffer, i.e. the number
					of sample frames that PortAudio will
					request from the callback. Many apps
					may want to use
					paFramesPerBufferUnspecified, which
					tells PortAudio to pick the best,
					possibly changing, buffer size.*/
					patestCallback, /* this is your callback function */
					&data); /*This is a pointer that will be passed to
							your callback*/
	if (err != paNoError)
	{
		this->print_error();
		return false;
	}
	err = Pa_StartStream(stream);
	if (err != paNoError)
	{
		this->print_error();
		return false;
	}
	return true;
}

const bool midi::audio::close()
{
	err = Pa_StopStream(stream);
	if (err != paNoError)
	{
		this->print_error();
		return false;
	}

	err = Pa_CloseStream(stream);
	if (err != paNoError)
	{
		this->print_error();
		return false;
	}
	return true;
}

void midi::audio::print_error()
{
#ifdef _DEBUG
	std::cerr << "PortAudio error:" << Pa_GetErrorText(err) << std::endl;
#endif // DEBUG
}

int midi::audio::version()
{
	return Pa_GetVersion();
}


const char * midi::audio::verson_text()
{
	return  Pa_GetVersionText();
}

const char * midi::audio::error_text()
{
	return Pa_GetErrorText(err);
}

const bool midi::audio::is_stoped()
{
	return Pa_IsStreamStopped(stream);
}

const bool midi::audio::is_active()
{
	return Pa_IsStreamActive(stream);
}

const PaStreamInfo * midi::audio::info()
{
	return  Pa_GetStreamInfo(stream);
}

const PaTime midi::audio::time()
{
	return Pa_GetStreamTime(stream);
}

const int midi::audio::device_count()
{
	auto count =  Pa_GetDeviceCount();
	if (count < 0)
	{
		count == 0;
		this->print_error();
	}
	return count;
}

const PaDeviceInfo * midi::audio::device_info(const int index)
{
	if (index >= device_count())
		return nullptr;
	return Pa_GetDeviceInfo(index);
}
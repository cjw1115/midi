#include "audio.h"
#include <iostream>

midi::audio::audio()
{
	this->_valid = true;
	auto err = Pa_Initialize();
	if (err != paNoError)
	{
#ifdef _DEBUG
	 std::cerr << "PortAudio error:" << Pa_GetErrorText(err) << std::endl;
#endif // DEBUG
		this->_valid = false;
	}
}

midi::audio::~audio()
{
	auto err = Pa_Terminate();
	if (err != paNoError)
	{
#ifdef _DEBUG
		std::cerr << "PortAudio error:" << Pa_GetErrorText(err) << std::endl;
#endif // DEBUG
	}
}

std::atomic_bool midi::audio::valid()
{
	return this->_valid;
}
#pragma once
#include <portaudio.h>
#include <atomic>

namespace midi
{
	class audio
	{
	//typedef int PaStreamCallback(const void *input,
	//		void *output,
	//		unsigned long frameCount,
	//		const PaStreamCallbackTimeInfo* timeInfo,
	//		PaStreamCallbackFlags statusFlags,
	//		void *userData);
	//typedef struct
	//{
	//	float left_phase;
	//	float right_phase;
	//}
	//paTestData;

	public:
		audio();
		~audio();
		std::atomic_bool valid();

	private:
		std::atomic_bool _valid;
	};
}
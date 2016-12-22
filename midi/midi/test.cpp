#include <iostream>
#include "audio.h"
using namespace std;

int main()
{
	{
		midi::audio audio;	
		auto count = audio.device_count();
		while(count --)
		{
			auto info = audio.device_info(count);
			cout << info->name << endl;
		}
	}
	system("pause");
	return 0;
}
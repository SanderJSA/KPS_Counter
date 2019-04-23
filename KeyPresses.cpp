//
// Created by SanderJSA on 01/04/2019.
//

#include "KeyPresses.h"
#include <FL/Fl.H>

#ifdef _WIN32
#include <Windows.h>
#include <WinUser.h>
#include <thread>

#else
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>

#endif

void windowsLoop(int &curPresses);
void linuxLoop(int &curPresses);

void getKeyPresses(int &curPresses)
{
#ifdef _WIN32
	windowsLoop(curPresses);
#else
	linuxLoop(curPresses);
#endif
}

#ifdef _WIN32
void windowsLoop(int &curPresses)
{
	while (true)
	{
		//Reset everything
		if (GetAsyncKeyState(VK_DELETE) & 0x0001)
		{
			Fl::lock();
			curPresses = -1;
			Fl::unlock();
		}
		else
		{
			for (int i = 0; i <= 223; i++)
			{
				if (GetAsyncKeyState(i) & 0x0001)
				{
					Fl::lock();
					curPresses++;
					Fl::unlock();
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

#else

struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
};

void linuxLoop(int &curPresses)
{
	//You might need to change "/dev/input/event3" if it doesn't work
	//If you want to find other available input device use the following command:
	//grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=1[02]001[3Ff]' | grep -Eo 'event[0-9]+'
	int input_fd = open("/dev/input/event3", O_RDONLY);

	input_event event{};
	while (read(input_fd, &event, sizeof(input_event)) > 0)
	{
		if (event.type == 1 && event.value == 1)
		{
			Fl::lock();
			//Reset everything after pressing delete
			if (event.code == 111)
			{
				curPresses = -1;
			}
			else
			{
				curPresses++;
			}
			Fl::unlock();
		}
	}
}

#endif
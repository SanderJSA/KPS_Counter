//
// Created by SanderJSA on 01/04/2019.
//

#include "KeyPresses.h"

#ifdef _WIN32
#include <Windows.h>
#include <WinUser.h>
#include <FL/Fl.H>
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
			continue;
		}

		//A S l ;
		if (GetAsyncKeyState(0x41) & 0x0001 ||
			GetAsyncKeyState(0x53) & 0x0001 ||
			GetAsyncKeyState(0x4C) & 0x0001 ||
			GetAsyncKeyState(VK_OEM_1) & 0x0001)
		{
			Fl::lock();
			curPresses++;
			Fl::unlock();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}
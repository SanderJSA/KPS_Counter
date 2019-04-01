#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Button.H>
#include <memory>

#ifdef _WIN32
#include <Winuser.h>
#else
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#endif

#define TIMER_DELAY 0.005

long totalPresses = 0;

Fl_Value_Output *setupGUI(int argc, char **argv);
void setupCB(Fl_Value_Output *window);
void windowsCB(void *data);
void linuxCB(void *data);

int main(int argc, char **argv)
{

	//Setup GUI and variables
	auto *window = setupGUI(argc, argv);

	//Creating timer callback
	setupCB(window);

	//Run program
	return Fl::run();
}


Fl_Value_Output *setupGUI(int argc, char **argv)
{
	Fl_Window *window = new Fl_Window(500,500);
	Fl_Value_Output *totPressesCounter = new Fl_Value_Output(10, 20, 100, 40);
	totPressesCounter->textsize(20);
	totPressesCounter->label("                   Total number of keypresses");
	totPressesCounter->align(FL_ALIGN_TOP);
	totPressesCounter->value(totalPresses);

	window->end();
	window->show(argc, argv);

	return totPressesCounter;
}

struct Data
{
	int input_fd;
	Fl_Value_Output *windows;
};

void setupCB(Fl_Value_Output *window)
{
#ifdef _WIN32
	Fl::add_timeout(TIMER_DELAY, windowsCB);
#else
	int input_fd = open("/dev/input/event3", O_RDONLY);
	if (input_fd == -1) {
			error(EXIT_FAILURE, errno, "Error opening input event device");
	}
	Fl::add_timeout(TIMER_DELAY, linuxCB, new Data{input_fd, window});
#endif
}

#ifdef _WIN32
void windowsCB(void *data)
{
	if (GetAsyncKeyState(VK_SPACE)!= 0)
	{
		totalPresses++;
	}



	Fl::repeat_timeout(TIMER_DELAY, windowsCB, data);
}

#else

struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
};

void linuxCB(void *data)
{
	input_event event{};
	while (read(((Data*)data)->input_fd, &event, sizeof(input_event)) > 0)
	{
		totalPresses++;
		((Data*)data)->windows->value(totalPresses);
		((Data*)data)->windows->redraw();
	}

	Fl::repeat_timeout(TIMER_DELAY, linuxCB, data);
}
#endif

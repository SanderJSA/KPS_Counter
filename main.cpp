#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Button.H>
#include <thread>
#include "KeyPresses.h"

#define TIMER_DELAY 0.1
#define ARRAY_SIZE (int)(1/TIMER_DELAY)

//Variables
long totalPresses = 0;
int curPresses = 0;
int pressesPerSecond[ARRAY_SIZE];
int curPosition = 0;

struct Data{
	Fl_Value_Output *totPressesCounter;
	Fl_Value_Output *kpsCounter;
};

Data *setupGUI(int argc, char **argv);
void processKeyPresses(void *data);

int main(int argc, char **argv)
{
	//Setup GUI and variables
	auto *data = setupGUI(argc, argv);

	//Creating timer callback
	Fl::add_timeout(TIMER_DELAY, processKeyPresses, data);

	//Launching separate thread
	std::thread keyThread(getKeyPresses, std::ref(curPresses));

	//Run GUI
	return Fl::run();
}


Data *setupGUI(int argc, char **argv)
{
	auto *window = new Fl_Window(500,500);

	//Creating total presses counter
	auto *totPressesCounter = new Fl_Value_Output(10, 20, 100, 40);
	totPressesCounter->textsize(20);
	totPressesCounter->label("Total key presses");
	totPressesCounter->align(FL_ALIGN_TOP_LEFT);

	//Creating keys per second counter
	auto *kpsCounter = new Fl_Value_Output(10, 80, 100, 40);
	kpsCounter->textsize(20);
	kpsCounter->label("Average keys/second");
	kpsCounter->align(FL_ALIGN_TOP_LEFT);

	window->end();
	window->show(argc, argv);

	return new Data{totPressesCounter, kpsCounter};
}

void processKeyPresses(void *data)
{

	//Reset
	if (curPresses == -1)
	{
		curPresses = 0;
		totalPresses = 0;
	}

	//Process new keystrokes
	totalPresses += curPresses;
	pressesPerSecond[curPosition] = curPresses;
	int avgPresses = 0;
	for (int i : pressesPerSecond)
	{
		avgPresses += i;
	}

	//Reached limit, go back to 0
	curPosition++;
	if (curPosition == ARRAY_SIZE)
	{
		curPosition = 0;
	}


	//Update GUI
	((Data*)data)->totPressesCounter->value(totalPresses);
	((Data*)data)->kpsCounter->value(avgPresses);

	//Prepare for next callback
	curPresses = 0;
	Fl::repeat_timeout(TIMER_DELAY, processKeyPresses, data);
}
/*

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
*/

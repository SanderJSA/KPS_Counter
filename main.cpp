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
int pressesOnSecond[ARRAY_SIZE];
int curPosition = 0;
int avgPressesSec = 0;
long secondsPassed = 1;
int maxKPS = 0;

struct Data{
	Fl_Value_Output *totPressesCounter;
	Fl_Value_Output *kpsCounter;
	Fl_Value_Output *avgKPS;
	Fl_Value_Output *maxKPS;
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
	int status = Fl::run();

	//Exit thread and exit program
	keyThread.detach();
	return status;
}


Data *setupGUI(int argc, char **argv)
{
	auto *window = new Fl_Window(500,500, "KeyPerSecond Counter");

	//Creating total presses counter
	auto *totPressesCounter = new Fl_Value_Output(10, 20, 150, 40);
	totPressesCounter->textsize(20);
	totPressesCounter->label("Total key presses");
	totPressesCounter->align(FL_ALIGN_TOP_LEFT);

	//Creating keys per second counter
	auto *kpsCounter = new Fl_Value_Output(10, 80, 150, 40);
	kpsCounter->textsize(20);
	kpsCounter->label("Keys per second");
	kpsCounter->align(FL_ALIGN_TOP_LEFT);

	//Creating average keys per second counter
	auto *avgKPS = new Fl_Value_Output(10, 140, 150, 40);
	avgKPS->textsize(20);
	avgKPS->label("Average keys per second");
	avgKPS->align(FL_ALIGN_TOP_LEFT);

	//Creating maximum keys per second
	auto *maxKPS = new Fl_Value_Output(10, 200, 150, 40);
	maxKPS->textsize(20);
	maxKPS->label("Maximum keys per second");
	maxKPS->align(FL_ALIGN_TOP_LEFT);

	window->end();
	window->show(argc, argv);

	return new Data{totPressesCounter, kpsCounter, avgKPS, maxKPS};
}

void processKeyPresses(void *data)
{
	//Reset
	if (curPresses == -1)
	{
		curPresses = 0;
		totalPresses = 0;
		secondsPassed = 1;
		maxKPS = 0;
	}

	//Process new keystrokes
	totalPresses += curPresses;
	avgPressesSec -= pressesOnSecond[curPosition];
	pressesOnSecond[curPosition] = curPresses;
	avgPressesSec += pressesOnSecond[curPosition];

	if (maxKPS < avgPressesSec)
	{
		maxKPS = avgPressesSec;
	}

	//Reached limit, go back to 0
	curPosition++;
	if (curPosition == ARRAY_SIZE)
	{
		if (avgPressesSec != 0)
		{
			secondsPassed++;
		}
		curPosition = 0;
	}

	//Update GUI
	((Data*)data)->totPressesCounter->value(totalPresses);
	((Data*)data)->kpsCounter->value(avgPressesSec);
	((Data*)data)->avgKPS->value((double)totalPresses/secondsPassed);
	((Data*)data)->maxKPS->value(maxKPS);

	//Prepare for next callback
	curPresses = 0;
	Fl::repeat_timeout(TIMER_DELAY, processKeyPresses, data);
}


/*
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

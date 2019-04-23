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

Fl_Value_Output *createCounter(int position);

Data *setupGUI(int argc, char **argv)
{
	auto *window = new Fl_Window(250,410, "KPS Counter");

	//Creating total presses counter
	auto *totPressesCounter = createCounter(0);
	totPressesCounter->label("Total key presses");

	//Creating keys per second counter
	auto *kpsCounter = createCounter(1);
	kpsCounter->label("Keys per second");

	//Creating average keys per second counter
	auto *avgKPS = createCounter(2);
	avgKPS->label("Average keys per second");

	//Creating maximum keys per second
	auto *maxKPS = createCounter(3);
	maxKPS->label("Maximum keys per second");

	window->end();
	window->show(argc, argv);

	return new Data{totPressesCounter, kpsCounter, avgKPS, maxKPS};
}

Fl_Value_Output *createCounter(int position)
{
	int const interval = 100;
	auto counter = new Fl_Value_Output(10, 20 + interval * position, 230, interval - 20);
	counter->textsize(30);
	counter->align(FL_ALIGN_TOP_LEFT);
	return counter;
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
		curPosition = 0;
		avgPressesSec = 0;
		for (int i = 0; i < ARRAY_SIZE; i++)
			pressesOnSecond[i] = 0;
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

	//Reached array limit, going back to 0
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
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Button.H>

#define TIMER_DELAY 0.005

long totalPresses = 0;

void setupGUI(int argc, char **argv);
void setupCB();
void windowsCB(void *data);
void linuxCB(void *data);

int main(int argc, char **argv) {

	//Setup GUI and variables
	setupGUI(argc, argv);

	//Creating timer callback
	setupCB();

	return Fl::run();
}

void setupGUI(int argc, char **argv)
{
	Fl_Window *window = new Fl_Window(500,500);
	Fl_Value_Output *totPressesCounter = new Fl_Value_Output(10, 20, 100, 40);
	totPressesCounter->textsize(20);
	totPressesCounter->label("                   Total number of keypresses");
	totPressesCounter->align(FL_ALIGN_TOP);

	window->end();
	window->show(argc, argv);
}

void setupCB()
{
#ifdef _WIN32
	Fl::add_timeout(TIMER_DELAY, windowsCB);
#else
	int input_fd = open("/dev/input/event3", O_RDONLY);
	if (input_fd == -1) {
			error(EXIT_FAILURE, errno, "Error opening input event device");
	}
	Fl::add_timeout(TIMER_DELAY, linuxCB, &input_fd);
#endif
}

void windowsCB(void *data)
{
	if (GetAsyncKeyState(VK_SPACE)!= 0)
	{
		totalPresses++;
	}



	Fl::repeat_timeout(TIMER_DELAY, windowsCB, data);
}

struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
};
/*

void linuxCB(void *data)
{
	int input_fd = *(int*)data;
	input_event event{};
		while (read(input_fd, &event, sizeof(input_event)) > 0) {
			std::cout << "input_event { .time = { .tv_sec = " << event.time.tv_sec
			          << ", .tv_usec = " << event.time.tv_usec
			          << " }, .type = " << event.type
			          << ", .code = " << event.code
			          << ", .value = " << event.value << " }" << std::endl;
		}
	}
}*/

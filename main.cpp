#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Output.H>
#include <Fl/Fl_Button.H>

#include <iostream>

long totalPresses = 0;

void whenPushed( Fl_Widget* o, void* counter);


int main(int argc, char **argv) {
	//Initial variables

	//Creating window
	Fl_Window *window = new Fl_Window(500,500);
	Fl_Value_Output *totPressesCounter = new Fl_Value_Output(10, 20, 100, 40);
	totPressesCounter->textsize(20);
	totPressesCounter->label("Total number of keypresses");
	totPressesCounter->align(FL_ALIGN_TOP);

	Fl_Button *button = new Fl_Button(10, 60, 100, 40);

	button->when(FL_WHEN_CHANGED);
	button->callback(whenPushed, totPressesCounter);



	window->end();
	window->show(argc, argv);

	return Fl::run();
}

void whenPushed( Fl_Widget* o, void* counter)
{
	if (((Fl_Button*)o)->value() == 1)
	{
		std::cout << Fl::event_original_key() << std::endl;
		totalPresses++;
		((Fl_Value_Output *) counter)->value(totalPresses);
	}
}
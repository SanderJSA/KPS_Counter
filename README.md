# KPSCounter
A multiplatform keys per seconds counter
It currently displays:
- The total number of keypresses
- Current keys per second
- The average keys per second
- The maxime keys per second

![alt text](https://github.com/SanderJSA/KPS_Counter/blob/master/Preview.png "What it looks like in i3-gaps")

Pressing Delete resets the counter.  
Pressing Escape while focused closes the program.  

## To install and run on linux:

Make sure you have the latest version of FLTK installed

> git clone https://github.com/SanderJSA/KPS_Counter  
> cd KPS_Counter  
> cmake CMakeLists.txt  
> make

To run it:
> sudo ./KPSCounter  

## To install and run on windows:

FLTK is required to compile.  

Using Clion:
>This is a CMake based project.  
>import the project in Clion and compile.  

Using another IDE:
>Import the source files and make sure FLTK can be included.  
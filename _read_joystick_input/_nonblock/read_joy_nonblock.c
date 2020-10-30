/**
 * @file read_joy_nonblock.c
 *
 * Read input from a joystick (/dev/input/js0)
 * Created w/aid of Joystick API Documentation
 * For BeagleBone Blue
 *
 * Manuel Saldana
 *
 */

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/input.h>
#include <linux/joystick.h>

int main()
{
	// INIT
	// open device [in non-blocking mode]
	// fd is file descriptor
	int fd = open("/dev/input/js0", O_NONBLOCK);

	// IOCTLs
	// read number of axes
	char number_of_axes;
	ioctl(fd, JSIOCGAXES, &number_of_axes);
	// read number of buttons
	char number_of_buttons;
	ioctl(fd, JSIOCGBUTTONS, &number_of_buttons);

	// OTHER VARIABLES [NOT PART OF API DOC]
	// create pointers for axes and buttons
	int *axis;
	char *button;
	// allocate memory for pointers
	axis = calloc(number_of_axes, sizeof(int));
	button = calloc(number_of_buttons, sizeof(char));

	// WHILE LOOP
	// loop that will continue to run
	while(1)
	{

		// EVENT READING
		// creating a js_event struct called e
		// (e is of type js_event)
		// js_event is defined in joystick.h
		struct js_event e;
		// read data previously written to a file
		// read sizeof(e) bytes from file descriptor fd into buffer pointed to by &e
		// if read returns -1, then there are no events pending to be read
		while(read(fd, &e, sizeof(e)) > 0)
		{
			// process event
		}
		// EAGAIN is returned when que is empty
		if(errno != EAGAIN)
		{
			// error
		}

		// JS_EVENT.TYPE
		// possible values for type are defined in joystick.h
		//		button pressed
		//		joystick moves
		//		init state of device

		// JS_EVENT.NUMBER
		// values of number correspond to axis or button that generated event
		// values vary from one joystick to another

		// JS_EVENT.VALUE
		// for axis, value is position of joystick along axis
		// axis values range from -32767 to 32767
		// for buttons, value is state of button
		// button values range from 0 to 1

		// JS_EVENT.TIME
		// values of time correspond to the time an event was generated

		// DETERMINE AND ASSIGN [NOT PART OF API DOC]
		// determine if event is of type button or axis
		// assign value to corresponding button or axis
		switch(e.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_BUTTON:
				button[e.number] = e.value;
				break;

			case JS_EVENT_AXIS:
				axis[e.number] = e.value;
				break;
		}

		// DISPLAY VALUES
		// move to beginning of current line
		printf("\r");
		// display axis numbers and values
		if(number_of_axes)
		{
			printf("AXES: ");
			for(int i = 0; i < number_of_axes; i++)
			{
				printf("%2d:%6d ", i, axis[i]);
			}
		}
		// display button numbers and values
		if(number_of_buttons)
		{
			printf("BUTTONS: ");
			for(int i = 0; i < number_of_buttons; i++)
			{
				printf("%2d:%s ", i, button[i] ? "1":"0");
			}
		}
		// flush
		fflush(stdout);

	}

}
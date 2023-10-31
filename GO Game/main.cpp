#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */

int main() {
	int zn = 0, x = 40, y = 12, attr = 7, back = 0, zero = 0;
	char txt[32];
	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("First name, Last Name, Student number");

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
	do {
		// we set black to be the background color
		// check conio2.h for available colors
		textbackground(BLACK);
		// clear the screen: we fill it out with spaces with
		// the specified background color
		clrscr();
		// we set the text color (7 == LIGHTGRAY)
		textcolor(7);
		// we move the coursor to column 48 and row 1
		// rows and column are numbered starting with 1
		gotoxy(48, 1);
		// we print out a text at a given cursor position
		// the cursor will move by the length of the text
		cputs("q       = exit");
		gotoxy(48, 2);
		cputs("cursors = moving");
		gotoxy(48, 3);
		cputs("space   = change color");
		gotoxy(48, 4);
		cputs("enter   = change background color");
		// print out the code of the last key pressed
		if(zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
		else sprintf(txt, "key code: 0x%02x", zn);
		gotoxy(48, 5);
		cputs(txt);

		// we draw a star
		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		// putch prints one character and moves the cursor to the right
		putch('*');

		// we wait for key press and get its code
		// most key codes correspond to the characters, like
		// a is 'a', 2 is '2', + is '+', but some special keys
		// like cursors provide two characters, where the first
		// one is zero, e.g., "up arrow" is zero and 'H'
		zero = 0;
		zn = getch();
		// we do not want the key 'H' to play role of "up arrow"
		// so we check if the first code is zero
		if(zn == 0) {
			zero = 1;		// if this is the case then we read
			zn = getch();		// the next code knowing that this
			if(zn == 0x48) y--;	// will be a special key
			else if(zn == 0x50) y++;
			else if(zn == 0x4b) x--;
			else if(zn == 0x4d) x++;
		} else if(zn == ' ') attr = (attr + 1) % 16;
		else if(zn == 0x0d) back = (back + 1) % 16;	// enter key is 0x0d or '\r'
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
					// visible after the program ends
	return 0;
	}

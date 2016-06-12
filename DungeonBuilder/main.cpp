

#include <string>
#include "main_menu.h"
#include "lib.h"
#include <Windows.h>
using namespace std;

void curses_init()
{	
	initscr();
	noecho();
	cbreak();
	keypad(stdscr,true);
	start_color();


//This function only exists in ncurses, not pdcurses
//and is necessary or there is a long delay on ESC key
#ifndef _WIN32
	set_escdelay(25);
#endif
}

int main(int argc,char* argv[])
{

	curses_init();

	MainMenu mm;
	mm.load();

	endwin();
	return 0;
}

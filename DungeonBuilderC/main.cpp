

#include <string>
#include "main_menu.h"
#include "lib.h"

using namespace std;

void curses_init()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr,true);
	start_color();
	set_escdelay(25);
}

int main(int argc,char* argv[])
{
	curses_init();

	MainMenu mm;
	mm.load();

	endwin();
	return 0;
}

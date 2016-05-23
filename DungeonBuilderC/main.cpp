#ifdef _WIN32
#include "curses.h"
#else
#include <ncurses.h>
#endif
#include <string>
//#include "text_editor.h"
#include "main_menu.h"

using namespace std;

void curses_init()
{	
	initscr();	
	noecho();
	cbreak();
	keypad(stdscr,true);	
	start_color();
}

int main(int argc,char* argv[])
{
	curses_init();
			
	
	MainMenu mm;
	mm.load();

	endwin();
	return 0;

}
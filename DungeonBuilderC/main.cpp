#include "headers/curses.h"
#include <string>
#include "headers/dungeon_editor.h"

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
	editor e = editor();
		
	e.edit("Dungeon Editor","Start");
	
	endwin();
	return 0;

}
#include "headers/curses.h"
#include <string>
//#include "headers/dungeon_editor.h"
#include "headers/main_menu.h"

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
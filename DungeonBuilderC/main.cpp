#include "headers/curses.h"
#include <string>
//#include "headers/dungeon_editor.hpp"
#include "headers/main_menu.hpp"

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
			
	
	mainMenu mm;
	mm.load();



	endwin();
	return 0;

}
#include "headers/main_menu.hpp"
#include "headers/command_window.hpp"

using namespace std;



void mainMenu::reset()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void mainMenu::load()
{

	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS,0,0);
	getmaxyx(stdscr,h,w); //why the fuck doesn't this work?
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	
	int done = false;
	string command;
	
	mvwprintw(mainWindow,0,0,"Dungeon Builder");	
	wrefresh(mainWindow);
			
	CommandWindow cmdW;
	cmdW.command(commandWindow,":");


}





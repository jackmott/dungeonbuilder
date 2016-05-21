#include "headers/main_menu.hpp"

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
	while(true)
	{
		mvwprintw(mainWindow,0,0,"Dungeon Builder");
		mvwprintw(commandWindow,0,0,":");
		wrefresh(mainWindow);
		//wmove(commandWindow,0,10);
	//	wclrtoeol(commandWindow);
		int input = wgetch(commandWindow);
		done = parseCommand(input,&command);
		if(done) {
			mvwprintw(responseWindow,0,0,command.c_str());
		}
	}

}





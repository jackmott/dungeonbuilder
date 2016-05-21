#include "headers/main_menu.hpp"
#include "headers/command_window.hpp"
#include "headers/dungeon_editor.hpp"
using namespace std;

void exit()
{

}

void create()
{
	DungeonEditor ed;
	ed.edit("Empty Room","Empty Room");
}


void mainMenu::reset()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void mainMenu::load()
{
	cmdMap["create"] = &create;

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
	bool cmdFound = false;
	string cmd;
	while (!cmdFound) {
		cmd = cmdW.command(commandWindow,":");
		cmdFound = cmdMap.count(cmd) > 0;		
		if(!cmdFound) {
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
	}

	commandFunction cmdFunc = cmdMap[cmd];
	cmdFunc();


	reset();
}






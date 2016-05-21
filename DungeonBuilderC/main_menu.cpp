#include "headers/main_menu.hpp"
#include "headers/command_window.hpp"
#include "headers/dungeon_editor.hpp"
#include "headers/printutlls.hpp"
#include "headers/utils.hpp"

using namespace std;

void exit(vector<string> args)
{

}

void create(vector<string> args)
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
	cmdMap["exit"] = &exit;

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
	
	setcolor(mainWindow,1,COLOR_RED);
	mvwprintwCenter(mainWindow,3,"Dungeon Builder");		
	setcolor(mainWindow,2,COLOR_CYAN);
	mvwprintwCenter(mainWindow,5,"[Create] a new Dungeon");	
	mvwprintwCenter(mainWindow,6,"[Exit] this world");	
	wrefresh(mainWindow);
	

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while (!cmdFound) {
		cmd = cmdW.command(commandWindow,":");
		if (cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;		
		}
		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
	}

	commandFunction cmdFunc = cmdMap[cmd[0]];
		
	cmdFunc(cmd);


	reset();
}






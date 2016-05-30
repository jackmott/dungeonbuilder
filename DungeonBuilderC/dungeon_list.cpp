#include "dungeon_list.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "command_window.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;


void DungeonList::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void DungeonList::resetWindows()
{
	commandWindow = newwin(1,getCols(),LINES-1,0);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	mainWindow = newwin(LINES-2,getCols()-8,0,4);
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	string command;

	int lineCount = 1;
	setcolors(mainWindow,lineCount,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"SELECT");

	for(auto e : *entities)
	{
		lineCount++;
		mvwprintw(mainWindow,lineCount,0,e->getPrimaryName().c_str());
	}


	wrefresh(mainWindow);

}

DungeonEntity* DungeonList::load(void *_entities)
{
	
	entities = (vector<DungeonEntity*>*) _entities;
	
	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;
		}
		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
		else
		{
			if(cmd[0] == STR_EXIT) break;
			commandFunction cmdFunc = cmdMap[cmd[0]];
			string response = (this->*cmdFunc)(cmd);
			if(response.length() > 0) {
				cmd.clear();
				mvwprintw(responseWindow,0,0,response.c_str());
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}
		}
	}


	clearWindows();
	return nullptr;
}






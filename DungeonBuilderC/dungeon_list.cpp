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


DungeonRoom* DungeonRoomList::pickRoom(int id)
{
	for(auto r : rooms)
	{
		if(r->uid == id)
		{
			return r;
		}
	}
	return nullptr;
}

DungeonRoom* DungeonRoomList::newRoom(vector<string> args)
{
	if(args.size() < 2)
	{
		return nullptr;
	}

	DungeonRoom *newRoom = new DungeonRoom();
	newRoom->setPrimaryName(join(1,args," "));
	return newRoom;
}

void DungeonRoomList::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void DungeonRoomList::resetWindows()
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

	setcolor(mainWindow,1,COLOR_RED);
	if(fromExit != nullptr){
		string fromTxt = fromExit->fromRoom->getPrimaryName()+ " -> ";
		string exitTxt = fromExit->getPrimaryName() + " -> ";
		string endTxt = "Select or [New](Name)";

		int w = getmaxx(mainWindow);
		int startX = (w- (fromTxt.size()+exitTxt.size()+endTxt.size()))/2;
		
		mvwprintw(mainWindow,lineCount,startX,fromTxt.c_str());
		startX += fromTxt.size();
		mvwprintwBold(mainWindow,lineCount,startX,exitTxt.c_str());
		startX += exitTxt.size();
		mvwprintw(mainWindow,lineCount,startX,endTxt.c_str());
	}
	else {
		mvwprintwCenterBold(mainWindow,lineCount,"Select or [New](Name)");
	}


	setcolor(mainWindow,2,COLOR_WHITE);
	int idWidth = 5;
	//print all the rooms
	for(auto r : rooms)
	{
		lineCount++;
		string id = to_string(r->uid);
		int spaces = idWidth - id.length();
		mvwprintwBold(mainWindow,lineCount,0,id.c_str());
		mvwprintw(mainWindow,lineCount,spaces,r->getPrimaryName().c_str());
		//print each rooms immediate exits
		for(auto e : r->exits)
		{
			if(e->room != nullptr)
			{
				lineCount++;
				string row = e->getPrimaryName() +" -> "+ e->room->getPrimaryName();
				mvwprintw(mainWindow,lineCount,idWidth+2,row.c_str());
			}
		}
	}


	wrefresh(mainWindow);

}

DungeonRoom* DungeonRoomList::load(vector<DungeonRoom *> _rooms,DungeonExit* _fromExit)
{

	rooms = _rooms;
	fromExit = _fromExit;

	cmdMap[STR_NEW] = &DungeonRoomList::newRoom;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);

		//check if input was a number
		char *p;
		long id = strtol(cmd[0].c_str(),&p,10);
		if(! *p)
		{
			DungeonRoom* resultRoom = pickRoom(id);
			if(resultRoom != nullptr)
			{
				return resultRoom;
			}
			else {
				mvwprintw(responseWindow,0,0,"Id not found");
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}
		}


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
			DungeonRoom* room = (this->*cmdFunc)(cmd);
			if(room != nullptr)
			{
				return room;
			}
			else {
				cmd.clear();
				mvwprintw(responseWindow,0,0,"Provide a room name");
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}



		}
	}


	clearWindows();
	return nullptr;
}






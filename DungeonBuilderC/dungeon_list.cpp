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


DungeonRoom* DungeonRoomList::pageUp(vector<string> args)
{
	pos -= 3;
	if (pos < 0 ) pos = 0;
	resetWindows();
	return nullptr;
}

DungeonRoom* DungeonRoomList::pageDown(vector<string> args)
{
	pos += 3;
	if (pos > rooms.size() - getRows() - 5) pos -= 3;
	resetWindows();
	return nullptr;
}


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
	delwin(headerWindow);
}

void DungeonRoomList::resetWindows()
{
	commandWindow = newwin(1,getCols(),LINES-1,0);
	keypad(commandWindow,true);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	headerWindow = newwin(1,getCols(),0,0);
	mainWindow = newwin(LINES-3,getCols()-2,1,1);
	
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	string command;

	

	init_pair(1,COLOR_BLACK,COLOR_RED);
	wbkgd(headerWindow,COLOR_PAIR(1));
	wclrtoeol(headerWindow);
	
	if(fromExit != nullptr){
		string fromTxt = fromExit->fromRoom->getPrimaryName();
		string exitTxt = STR_RIGHT_ARROW;
		exitTxt += "EXIT:"+fromExit->getPrimaryName() + STR_RIGHT_ARROW;
		string endTxt = "[#/New](Name)";

		int w = getmaxx(headerWindow);
		int startX = (w- (fromTxt.size()+exitTxt.size()+endTxt.size()))/2;
		setcolors(headerWindow,2,COLOR_BLACK,COLOR_RED);
		mvwprintw(headerWindow,0,startX,fromTxt.c_str());
		startX += fromTxt.size();
		setcolors(headerWindow,3,COLOR_WHITE,COLOR_RED);
		mvwprintwBold(headerWindow,0,startX,exitTxt.c_str());
		startX += exitTxt.size();
		setcolors(headerWindow,2,COLOR_BLACK,COLOR_RED);
		mvwprintw(headerWindow,0,startX,endTxt.c_str());
	}
	else {
		mvwprintwCenterBold(headerWindow,0,"Select or [New](Name)");
	}

	int lineCount = 0;
	setcolor(mainWindow,4,COLOR_WHITE);
	int idWidth = 5;
	int numRows = getRows()-5;
	//print all the rooms
	for (int i = pos; i < numRows+pos; i++)
	{
		if(i >= rooms.size())
		{
			break;
		}
		DungeonRoom *r = rooms[i];
		lineCount++;
		string id = to_string(r->uid);
		
		mvwprintwBold(mainWindow,lineCount,0,id.c_str());
		mvwprintw(mainWindow,lineCount,idWidth,r->getPrimaryName().c_str());
		//print each rooms immediate exits
		for(auto e : r->exits)
		{
			if(e->room != nullptr)
			{
				lineCount++;
				string row = e->getPrimaryName() +STR_RIGHT_ARROW+ e->room->getPrimaryName();
				mvwprintw(mainWindow,lineCount,idWidth+2,row.c_str());
			}
		}
		
	}
	if(rooms.size() > numRows+pos) {
		mvwprintwCenter(mainWindow,numRows+1,"PgDown For More");
	}

	wrefresh(mainWindow);
	wrefresh(headerWindow);

}

DungeonRoom* DungeonRoomList::load(vector<DungeonRoom *> _rooms,DungeonExit* _fromExit)
{

	rooms = _rooms;
	fromExit = _fromExit;

	pos = 0;

	cmdMap[STR_NEW] = &DungeonRoomList::newRoom;
	cmdMap[STR_PAGE_UP] = &DungeonRoomList::pageUp;
	cmdMap[STR_PAGE_DOWN] = &DungeonRoomList::pageDown;
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
				clearWindows();
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
				clearWindows();
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





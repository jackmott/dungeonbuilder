#include "dungeon_exit.h"
#include "dungeon_list.h"
#include "dungeon_room.h"
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
	responseWindow = newwin(1,getCols(),LINES-2,0);
	headerWindow = newwin(1,getCols(),0,0);
	mainWindow = newwin(LINES-3,getCols(),1,0);
	
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	string command;

	

	
	if(fromExit != nullptr){
		printHeader(headerWindow,fromExit->parent->getPrimaryName(),"EXIT:"+fromExit->getPrimaryName(),STR_MENU_ID_OR_NEW);		
	}
	else {
		printHeader(headerWindow,STR_MENU_ID_OR_NEW);		
	}

	int lineCount = 0;
	setcolor(mainWindow,1,COLOR_WHITE);
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






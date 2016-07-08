#include "dungeon_exit.h"
#include "room_list.h"
#include "dungeon_room.h"
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
	newRoom->setPrimaryName(join(1,args,CHR_SPACE));
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
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	headerWindow = newwin(1,COLS,0,0);
	mainWindow = newwin(LINES-3,COLS,1,0);

	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	if(fromExit != nullptr){
		printHeader(headerWindow,"Exit Room List",fromExit->parent->getPrimaryName(),fromExit->getPrimaryName(),STR_MENU_ID_OR_NEW);
	}
	else {
		printHeader(headerWindow,"Room List",STR_MENU_ID_OR_NEW);
	}


	setcolor(mainWindow,COLOR_WHITE);
	size_t idWidth = 5;

	//print all the rooms
	for(size_t i = 0; i < rooms.size(); i++)
	{
		
		DungeonRoom *r = rooms[i];		
		string id = to_string(r->uid);

		while(id.size() < idWidth)
		{
			id += " ";
		}
				
		string roomStr = r->getPrimaryName();
		roomStr += " ("+ r->description.substr(0,30)+" ...)";
		textBuffer.push_back(id+roomStr);
		//print each rooms immediate exits
		for(auto e : r->exits)
		{
			if(e->room != nullptr)
			{				
				string row;
				row.insert(0,idWidth+2,' ');
				row += e->getPrimaryName() +STR_RIGHT_ARROW+ e->room->getPrimaryName();				
				textBuffer.push_back(row);
			}
		}

	}
	renderTextBuffer();

	wrefresh(mainWindow);
	wrefresh(headerWindow);

}

DungeonRoom* DungeonRoomList::load(const vector<DungeonRoom *> &_rooms,DungeonExit* _fromExit)
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

		if(cmd.size() > 0 && cmd[0].size() > 0)
		{
			if (checkCommonInput(cmd[0])) continue;
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






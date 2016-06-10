
#include "object_list.h"
#include "dungeon_object.h"
#include "command_window.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;


DungeonObject* DungeonObjectList::pageUp(vector<string> args)
{
	pos -= 3;
	if(pos < 0) pos = 0;
	resetWindows();
	return nullptr;
}

DungeonObject* DungeonObjectList::pageDown(vector<string> args)
{
	pos += 3;
	if(pos > objects.size() - getRows() - 5) pos -= 3;
	resetWindows();
	return nullptr;
}


DungeonObject* DungeonObjectList::pickObject(int id)
{
	for(auto o : objects)
	{
		if(o->uid == id)
		{
			return o;
		}
	}
	return nullptr;
}

DungeonObject* DungeonObjectList::newObject(vector<string> args)
{
	if(args.size() < 2)
	{
		return nullptr;
	}

	DungeonObject *newObject = new DungeonObject();
	newObject->setPrimaryName(join(1,args," "));
	return newObject;
}

void DungeonObjectList::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}

void DungeonObjectList::resetWindows()
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




	//if(fromExit != nullptr){
		//printHeader(headerWindow,fromExit->parent->getPrimaryName(),"EXIT:"+fromExit->getPrimaryName(),STR_MENU_ID_OR_NEW);		
	//}
	//else {
	printHeader(headerWindow,STR_MENU_ID_OR_NEW);
	//}

	int lineCount = 0;
	setcolor(mainWindow,1,COLOR_WHITE);
	int idWidth = 5;
	int numRows = getRows()-5;
	//print all the objects
	for(size_t i = pos; i < numRows+pos; i++)
	{
		if(i >= objects.size())
		{
			break;
		}
		DungeonObject *o = objects[i];
		lineCount++;
		string id = to_string(o->uid);

		mvwprintwBold(mainWindow,lineCount,0,id.c_str());
		mvwprintw(mainWindow,lineCount,idWidth,o->getPrimaryName().c_str());

	}
	if(objects.size() > numRows+pos) {
		mvwprintwCenter(mainWindow,numRows+1,"PgDown For More");
	}

	wrefresh(mainWindow);
	wrefresh(headerWindow);

}

DungeonObject* DungeonObjectList::load(vector<DungeonObject *> _objects)
{

	objects = _objects;

	pos = 0;

	cmdMap[STR_NEW] = &DungeonObjectList::newObject;
	cmdMap[STR_PAGE_UP] = &DungeonObjectList::pageUp;
	cmdMap[STR_PAGE_DOWN] = &DungeonObjectList::pageDown;
	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);

		//check if input was a number
		if(cmd[0].size() > 0)
		{
			char *p;
			long id = strtol(cmd[0].c_str(),&p,10);
			if(! *p)
			{
				DungeonObject* resultObject= pickObject(id);
				if(resultObject != nullptr)
				{
					clearWindows();
					return resultObject;
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
			DungeonObject* object = (this->*cmdFunc)(cmd);
			if(object != nullptr)
			{
				clearWindows();
				return object;
			}
			else {
				cmd.clear();
				mvwprintw(responseWindow,0,0,"Provide an object name");
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}

		}
	}


	clearWindows();
	return nullptr;
}






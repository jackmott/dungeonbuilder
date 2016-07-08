
#include "dungeon_exit.h"
#include "dungeon_entity.h"
#include "dungeon_action.h"
#include "dungeon_creature.h"
#include "dungeon_room.h"
#include "entity_list.h"
#include "dungeon_object.h"
#include "command_window.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;


DungeonEntity* EntityList::pageUp(vector<string> args)
{
	pos -= 3;
	if(pos < 0) pos = 0;
	resetWindows();
	return nullptr;
}

DungeonEntity* EntityList::pageDown(vector<string> args)
{
	pos += 3;
	if(pos > entities.size() - LINES - 5) pos -= 3;
	resetWindows();
	return nullptr;
}


DungeonEntity* EntityList::pickEntity(int id)
{
	for(auto e : entities)
	{
		if(e.entity != nullptr && e.entity->uid == id)
		{
			return e.entity;
		}
	}
	return nullptr;
}

DungeonEntity* EntityList::newObject(vector<string> args)
{
	if(args.size() < 2)
	{
		return nullptr;
	}

	DungeonObject *newObject = new DungeonObject();
	newObject->setPrimaryName(join(1,args,CHR_SPACE));
	return newObject;
}

void EntityList::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}

void EntityList::resetWindows()
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


	printHeader(headerWindow,"Entity List",STR_MENU_ID_OR_NEW);


	int lineCount = 0;
	setcolor(mainWindow,COLOR_WHITE);
	int idWidth = 5;
	int numRows = LINES-5;
	//print all the objects
	for(size_t i = pos; i < numRows+pos; i++)
	{
		if(i >= entities.size())
		{
			break;
		}
		EntityDisplay ed = entities[i];
		lineCount++;

		if (ed.entity != nullptr) {
			string id = to_string(ed.entity->uid);			
			mvwprintwBold(mainWindow,lineCount,ed.depth,id.c_str());
			mvwprintw(mainWindow,lineCount,idWidth+ed.depth,ed.s.c_str());
		} else {
			mvwprintw(mainWindow,lineCount,0,ed.s.c_str());
		}

	}
	if(entities.size() > numRows+pos) {
		mvwprintwCenter(mainWindow,numRows+1,"PgDown For More");
	}

	wrefresh(mainWindow);
	wrefresh(headerWindow);

}

void EntityList::loadAllObjects(std::vector<DungeonObject*> objects,int depth)
{
	if(depth == 0) {
		EntityDisplay ed;
		ed.entity = nullptr;
		ed.s = "Objects:";
		entities.push_back(ed);
	}
	for(auto e : room->objects)
	{
		EntityDisplay ed;
		ed.depth = 0;
		ed.entity = e;
		ed.s = e->getPrimaryName();
		entities.push_back(ed);

		loadAllObjects(e->contents,depth+1);
		loadAllObjects(e->ontops,depth+1);
	}

}
void EntityList::loadAllCreatures(std::vector<DungeonCreature*> creatures)
{
	EntityDisplay ed;
	ed.entity = nullptr;
	ed.s = "Creatures:";
	entities.push_back(ed);
	for(auto e : room->creatures)
	{
		EntityDisplay ed;
		ed.depth = 0;
		ed.entity = e;
		ed.s = e->getPrimaryName();
		entities.push_back(ed);
	}

}
void EntityList::loadAllActions(std::vector<DungeonAction*> actions)
{
	EntityDisplay ed;
	ed.entity = nullptr;
	ed.s = "Actions:";
	entities.push_back(ed);
	for(auto e : room->actions)
	{
		EntityDisplay ed;
		ed.depth = 0;
		ed.entity = e;
		ed.s = e->getPrimaryName();
		entities.push_back(ed);
	}
}
void EntityList::loadAllExits(std::vector<DungeonExit*> exits)
{
	EntityDisplay ed;
	ed.entity = nullptr;
	ed.s = "Exits:";
	entities.push_back(ed);
	for(auto e : room->exits)
	{
		EntityDisplay ed;
		ed.depth = 0;
		ed.entity = e;
		ed.s = e->getPrimaryName();
		entities.push_back(ed);
	}
}

DungeonEntity* EntityList::load(DungeonRoom* _room)
{

	room = _room;

	loadAllObjects(room->objects);
	loadAllCreatures(room->creatures);
	loadAllActions(room->actions);
	loadAllExits(room->exits);


	pos = 0;

	cmdMap[STR_NEW] = &EntityList::newObject;
	cmdMap[STR_PAGE_UP] = &EntityList::pageUp;
	cmdMap[STR_PAGE_DOWN] = &EntityList::pageDown;
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
				DungeonEntity* resultEntity= pickEntity(id);
				if(resultEntity != nullptr)
				{
					clearWindows();
					return resultEntity;
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
			DungeonEntity* object = (this->*cmdFunc)(cmd);
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






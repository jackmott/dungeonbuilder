#include "object_list.h"
#include "dungeon_creature.h"
#include "dungeon_object.h"
#include "dungeon_exit.h"
#include "dungeon_room.h"
#include "room_editor.h"
#include "command_window.h"
#include "text_editor.h"
#include "object_editor.h"
#include "creature_editor.h"
#include "exit_editor.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include "room_list.h"

using namespace std;

string RoomEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string RoomEditor::move(vector<string> args)
{
	if(args.size() < 2)
	{
		DungeonRoomList dl;
		DungeonRoom* result = dl.load(g_roomList);
		room = result;
	}
	else {

		string exitStr = join(1,args,CHR_SPACE);
		DungeonExit *e = (DungeonExit*)extractEntity(&room->exits,&exitStr);
		if(e != nullptr)
		{
			room = e->room;
		}
		else
		{
			return "I don't see that here.";
		}
	}

	clearWindows();
	resetWindows();
	return "";

}

string RoomEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}
	if(args.size() < 3) {
		return "Please supply the value directly in the command";
	}
	string setNoun = args[1];
	toLower(&setNoun);
	if(setNoun == STR_NAME)
	{
		string newname = join(2,args,CHR_SPACE);
		room->setPrimaryName(newname);
	}
	else if(setNoun == STR_DESCRIPTION || setNoun == STR_DESC)
	{
		string desc = join(2,args,CHR_SPACE);		
		room->description = desc;
	}
	else if(setNoun == STR_LIGHT)
	{
		room->hasLight = isAffirmative(args[2]);
	}
	else {
		return "I don't know how to set that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string RoomEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == STR_NAME)
	{
		return set(args);
	}
	else if(editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		TextEditor ed;
		room->description = ed.edit("Description For Room:"+room->getPrimaryName(),room->description);
	}
	else if(editNoun == STR_OBJECT)
	{
		if(args.size() <3)
		{
			//object not found round here, bring up the list!
			DungeonObjectList dol;
			DungeonObject* o = dol.load(g_objectList);
			ObjectEditor ed;
			ed.load(o);
		}
		else {
			string objStr = join(2,args,CHR_SPACE);
			DungeonObject *o = (DungeonObject*)extractEntity(&room->objects,&objStr);
			if(o != nullptr)
			{
				ObjectEditor ed;
				ed.load(o);
			}
			else
			{
				//object not found round here, bring up the list!
				DungeonObjectList dol;
				DungeonObject* o = dol.load(g_objectList);
				ObjectEditor ed;
				ed.load(o);

			}
		}
	}
	else if(editNoun == STR_CREATURE)
	{
		if(args.size() <3)
		{
			return "Which creature do you want to edit?";
		}
		string creatureStr = join(2,args,CHR_SPACE);
		DungeonCreature *c = (DungeonCreature*)extractEntity(&room->creatures,&creatureStr);
		if(c != nullptr)
		{
			CreatureEditor ed;
			ed.load(c);
		}
		else
		{
			return "I don't see that here.";
		}
	}
	else if(editNoun == STR_EXIT)
	{
		if(args.size() <3)
		{
			return "Which exit do you want to edit?";
		}
		string exitStr = join(2,args,CHR_SPACE);
		DungeonExit *e = (DungeonExit*)extractEntity(&room->exits,&exitStr);
		if(e != nullptr)
		{
			ExitEditor ed;
			DungeonRoom* newRoom = ed.load(e);
			if(newRoom != nullptr)
			{
				room = newRoom;
			}
		}
		else
		{
			return "I don't see that here.";
		}
	}
	else
	{
		return "I don't know how to edit that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string RoomEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string delNoun = args[1];
	toLower(&delNoun);


	if(delNoun == STR_OBJECT)
	{
		if(args.size() <3)
		{
			return "Which object do you want to delete?";
		}
		string objStr = join(2,args,CHR_SPACE);
		DungeonObject *o = (DungeonObject*)extractEntity(&room->objects,&objStr);
		if(o != nullptr)
		{
			removePointer(&room->objects,o);
			delete o;
			resetWindows();
			return "";
		}
		else
		{
			return "I don't see that here.";
		}
	}
	else if(delNoun == STR_CREATURE)
	{
		if(args.size() <3)
		{
			return "Which creature do you want to delete?";
		}
		string creatureStr = join(2,args,CHR_SPACE);
		DungeonCreature *c = (DungeonCreature*)extractEntity(&room->creatures,&creatureStr);
		if(c != nullptr)
		{
			removePointer(&room->creatures,c);
			delete c;
			resetWindows();
			return "";
		}
		else
		{
			return "I don't see that here.";
		}
	}
	else if(delNoun == STR_EXIT)
	{
		if(args.size() <3)
		{
			return "Which exit do you want to delete?";
		}
		string exitStr = join(2,args,CHR_SPACE);
		DungeonExit *e = (DungeonExit*)extractEntity(&room->exits,&exitStr);
		if(e != nullptr)
		{
			removePointer(&room->exits,e);
			delete e;
			resetWindows();
			return "";
		}
		else
		{
			return "I don't see that here.";
		}
	}
	else
	{
		return "I don't know how to delete that";
	}

}

string RoomEditor::add(vector<string> args)
{
	if(args.size() < 2) {
		return "What do you want to add?";
	}

	if(args.size() < 3)
	{
		return "Provide name for the " + args[1];
	}
	string addNoun = args[1];

	toLower(&addNoun);


	if(addNoun == STR_CREATURE)
	{
		CreatureEditor editor;
		DungeonCreature* creature = new DungeonCreature();
		creature->parent = room;
		creature->addName(join(2,args,CHR_SPACE));
		clearWindows();
		editor.load(creature);
		room->creatures.push_back(creature);
		resetWindows();
		return "";
	}
	else if(addNoun == STR_OBJECT)
	{
		ObjectEditor oe;
		DungeonObject* o = new DungeonObject();
		o->parent = room;
		o->addName(join(2,args,CHR_SPACE));
		clearWindows();
		oe.load(o);
		room->objects.push_back(o);
		resetWindows();

		return "";
	}
	else if(addNoun == STR_EXIT)
	{

		//Make a a new exit, add it to the room's list of exits
		DungeonExit * e = new DungeonExit();
		e->parent = room;
		e->addName(join(2,args,CHR_SPACE));
		room->exits.push_back(e);

		//Fire up the list picker with a list of rooms
		DungeonRoomList listPicker;
		clearWindows();
		DungeonRoom *newRoom = listPicker.load(g_roomList,e);

		//Once a room is picked, fire up the exit editor with the room
		if(newRoom != nullptr)
		{
			e->room = newRoom;
			ExitEditor ed;
			newRoom = ed.load(e);
			if(newRoom != nullptr)
			{
				//If a new room is returned, go to it
				room = newRoom;
			}

		}
		else
		{
			return "something has gone wrong.";
		}
		resetWindows();
		return "";
	}
	else {
		return "I don't know how to create that";
	}
}


void RoomEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
	clear();
}

void RoomEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-3,COLS,1,0);
	headerWindow = newwin(1,COLS,0,0);	
	getmaxyx(stdscr,h,w); // this doesn't work in windows


	printHeader(headerWindow,"ROOM:"+room->getPrimaryName());

	int lineCount = 2;
	string nameRow = STR_MENU_ROOM_NAME + room->getPrimaryName();
	mvwprintw(mainWindow,lineCount,0,nameRow.c_str());
	lineCount++;
	string desc = room->description.size() > (COLS - 20) ? room->description.substr(0,60) + STR_ELLIPSES : room->description;
	string descRow = STR_MENU_DESCRIPTION + desc;
	mvwprintw(mainWindow,lineCount,0,descRow.c_str());
	lineCount++;
	string torf = room->hasLight ? "T" : "F";
	string lightRow = STR_MENU_HAS_LIGHT + torf;
	mvwprintw(mainWindow,lineCount,0,lightRow.c_str());

	lineCount++;
	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_OBJECT);
	for(auto o : room->objects)
	{
		lineCount++;
		string row = o->getPrimaryName();
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_CREATURE);
	for(auto creature : room->creatures)
	{
		lineCount++;
		string row = creature->getPrimaryName();
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_EXIT);
	for(auto e : room->exits)
	{
		lineCount++;
		string row = e->getPrimaryName() + STR_RIGHT_ARROW + e->room->getPrimaryName();
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}
	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_ROOM_MOVE);


	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);


}

void RoomEditor::load(DungeonRoom *_room)
{
	room = _room;
	cmdMap[STR_EDIT] = &RoomEditor::edit;
	cmdMap[STR_SET] = &RoomEditor::set;
	cmdMap[STR_EXIT] = &RoomEditor::exit;
	cmdMap[STR_ADD] = &RoomEditor::add;
	cmdMap[STR_DELETE] = &RoomEditor::del;
	cmdMap[STR_GO] = &RoomEditor::move;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;

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
	}


	clearWindows();
}






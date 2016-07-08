#include "entity_list.h"
#include "dungeon_creature.h"
#include "action_editor.h"
#include "dungeon_action.h"
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
		DungeonRoom* result = dl.load(globalState.roomList);
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
			return "specify and object name";
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
				return "object not found";

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
	else if(editNoun == STR_ACTION)
	{
		if(args.size() <3)
		{
			return "Which action do you want to edit?";
		}
		string actionStr = join(2,args,CHR_SPACE);
		DungeonAction *a = (DungeonAction*)extractEntity(&room->actions,&actionStr);
		if(a != nullptr)
		{
			ActionEditor ed;
			ed.load(a);
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
	else if(delNoun == STR_ACTION) {

		//TODO - check if use alias is already used for summin else
		if(args.size() < 3)
		{
			return "Provide an alias to delete.";
		}
		string actionStr = join(2,args,CHR_SPACE);
		DungeonAction *a = (DungeonAction*)extractEntity(&room->actions,&actionStr);
		if(a != nullptr)
		{
			removePointer(&room->actions,a);
			delete a;
			return "";
		}
		return "That doesn't seem to exit.";
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

	if(addNoun == STR_ACTION) {

		if(args.size() < 3)
		{
			return "Provide a string to name the action.";
		}
		string actionStr = join(2,args,CHR_SPACE);
		DungeonAction *action = new DungeonAction();
		action->setPrimaryName(actionStr);
		action->parent = room;
		room->actions.push_back(action);
		ActionEditor ae;
		ae.load(action);
		resetWindows();
		return "";
	}
	else if(addNoun == STR_CREATURE)
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
        string name = join(2,args,CHR_SPACE);
		e->addName(name);
		room->exits.push_back(e);

		//Fire up the list picker with a list of rooms
		DungeonRoomList listPicker;
		clearWindows();
		DungeonRoom *newRoom = listPicker.load(globalState.roomList,e);

        

		//Once a room is picked, fire up the exit editor with the room
		if(newRoom != nullptr)
		{			
			e->room = newRoom;
			e->mirror();
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
	
	textBuffer.clear();

	printHeader(headerWindow,"Room",room->getPrimaryName());

	
	string nameRow = STR_MENU_ROOM_NAME + room->getPrimaryName();
	textBuffer.push_back(nameRow);
	
	string desc = room->description.size() > (size_t)(COLS - 20) ? room->description.substr(0,COLS-20) + STR_ELLIPSES : room->description;
	string descRow = STR_MENU_DESCRIPTION + desc;
	textBuffer.push_back(descRow);
	
	string torf = room->hasLight ? "T" : "F";
	string lightRow = STR_MENU_HAS_LIGHT + torf;
	textBuffer.push_back(lightRow);

	textBuffer.push_back(STR_MENU_OBJECT);
	
	for(auto o : room->objects)
	{
		string row = o->getPrimaryName();
		textBuffer.push_back("  "+row);		
	}

	textBuffer.push_back(STR_MENU_CREATURE);	
	for(auto creature : room->creatures)
	{
		
		string row = creature->getPrimaryName();
		textBuffer.push_back("  "+row);
	}

	textBuffer.push_back(STR_MENU_EXIT);	
	for(auto e : room->exits)
	{
	
		string row = e->getPrimaryName() + STR_RIGHT_ARROW + e->room->getPrimaryName();
		textBuffer.push_back("  "+row);
	}

	textBuffer.push_back(STR_MENU_ACTIONS);
	for(auto a : room->actions)
	{
		textBuffer.push_back("  "+a->getPrimaryName());
	}

	textBuffer.push_back(STR_MENU_ROOM_MOVE);
	
	renderTextBuffer();


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
			if (checkCommonInput(cmd[0])) continue;
			
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
				if(response.length() > 0 && response != STR_PAGE_UP && response != STR_PAGE_DOWN) {
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






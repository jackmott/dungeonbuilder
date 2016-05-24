
#include "room_editor.h"

using namespace std;

string RoomEditor::exit(vector<string> args)
{
	return STR_EXIT;
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
		string newname = join(2,args," ");
		room->name = newname;
	}
	else if(setNoun == STR_DESCRIPTION || setNoun == STR_DESC)
	{
		string desc = join(2,args," ");
		vector<string> descVector;
		descVector.push_back(desc);
		room->description = descVector;
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
		clearWindows();		
		room->description = ed.edit("Editing Description For Room:"+room->name,room->description);
		resetWindows();
		return "";
	}
	else
	{
		return "I don't know how to edit that";
	}

}

string RoomEditor::create(vector<string> args)
{
	if(args.size() < 2) {
		return "What do you want to create?";
	}

	if(args.size() < 3)
	{
		return "Provide name for the " + args[1];
	}
	string createNoun = args[1];

	toLower(&createNoun);


	if(createNoun == STR_CREATURE)
	{
		CreatureEditor editor;
		DungeonCreature* creature = new DungeonCreature();
		creature->name = join(2,args," ");
		clearWindows();
		editor.load(creature);
		room->creatures.push_back(creature);
		resetWindows();
		return "";
	}
	else if(createNoun == STR_OBJECT)
	{
		ObjectEditor oe;
		DungeonObject* o = new DungeonObject();
		o->name = join(2,args," ");
		clearWindows();
		oe.load(o);
		room->objects.push_back(o);
		resetWindows();

		return "";
	}
	else if(createNoun == STR_EXIT)
	{
		ExitEditor editor;
		DungeonExit * e = new DungeonExit();
		e->room = g_startRoom;
		e->name = join(2,args," ");
		clearWindows();
		DungeonRoom* newRoom = editor.load(e,room);
		room->exits.push_back(e);
		if(newRoom != NULL)
		{
			room = newRoom;
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
	clear();
}

void RoomEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS-8,0,4);
	scrollok(mainWindow,TRUE);
	getmaxyx(stdscr,h,w); // this doesn't work in windows

	int lineCount = 3;


	setcolors(mainWindow,1,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"Room Editor");
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = STR_MENU_NAME + room->name;
	mvwprintw(mainWindow,lineCount,0,nameRow.c_str());

	lineCount++;
	string desc = room->description.size() > 0 ? room->description[0] + STR_ELLIPSES : "";
	string descRow = STR_MENU_DESCRIPTION + desc;
	mvwprintw(mainWindow,lineCount,0,descRow.c_str());

	lineCount++;
	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_OBJECT);
	for(auto o : room->objects)
	{
		lineCount++;
		string row = o->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_CREATURE);
	for(auto creature : room->creatures)
	{
		lineCount++;
		string row = creature->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_EXIT);
	for(auto e : room->exits)
	{
		lineCount++;
		string row = e->name + STR_ARROW + e->room->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	refresh();
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
	cmdMap[STR_CREATE] = &RoomEditor::create;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.command(commandWindow,STR_PROMPT);
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






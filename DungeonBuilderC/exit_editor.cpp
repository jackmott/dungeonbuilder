
#include "exit_editor.h"

using namespace std;

string ExitEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string ExitEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == STR_NAME)
	{
		//set it directly or go to editor
		if(args.size() < 3) {
			return "Please supply the name directly in the command";
		}
		else
		{
			string newname = join(2,args," ");
			dungeonExit->name = newname;
			clearWindows();
			resetWindows();
			return "";
		}

	}
	else if (editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		TextEditor ed;
		string newdesc = ed.edit("Editing Description For Object:"+dungeonExit->name,dungeonExit->description);
		dungeonExit->description = newdesc;
		clearWindows();
		resetWindows();
		return "";
	}
	else if(editNoun == STR_ROOM)
	{
		return STR_ROOM;
	}
	else
	{
		return "I don't know how to edit that";
	}

}

string ExitEditor::create(vector<string> args)
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

	if(createNoun == STR_ROOM)
	{
		DungeonRoom* newRoom = new DungeonRoom();		
		newRoom->name = join(2,args," ");
		dungeonExit->room = newRoom;
		g_roomList.push_back(newRoom);
		clearWindows();
		resetWindows();		
		return "";
	}
	else {
		return "I don't know how to create that";
	}
}


void ExitEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void ExitEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS-8,0,4);
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	int done = false;
	string command;

	setcolors(mainWindow,1,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"Exit Editor");
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = STR_MENU_NAME + dungeonExit->name;
	mvwprintw(mainWindow,3,0,nameRow.c_str());
	string descRow = STR_MENU_DESCRIPTION + dungeonExit->description;
	mvwprintw(mainWindow,4,0,descRow.c_str());
	string roomRow = STR_MENU_ROOM + dungeonExit->room->name;
	mvwprintw(mainWindow,5,0,roomRow.c_str());


	wrefresh(mainWindow);

}

DungeonRoom* ExitEditor::load(DungeonExit *_dungeonExit, DungeonRoom *_fromRoom)
{
	dungeonExit = _dungeonExit;
	fromRoom = _fromRoom;
	cmdMap[STR_EDIT] = &ExitEditor::edit;
	cmdMap[STR_EXIT] = &ExitEditor::exit;
	cmdMap[STR_CREATE] = &ExitEditor::create;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.command(commandWindow,STR_PROMPT);
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

			commandFunction cmdFunc = cmdMap[cmd[0]];
			string response = (this->*cmdFunc)(cmd);
			if(response == STR_ROOM)
			{
				return dungeonExit->room;
			}
			else if(response == STR_EXIT)
			{
				return fromRoom;
			}
			if(response.length() > 0) {
				cmd.clear();
				mvwprintw(responseWindow,0,0,response.c_str());
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}
		}
	}


	clearWindows();
}





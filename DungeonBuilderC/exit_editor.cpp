
#include "exit_editor.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;

string ExitEditor::exit(vector<string> args)
{
	return STR_EXIT;
}



string ExitEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}
	if(args.size() < 3) {
		return "Please supply the value directly in the command";
	}
	string editNoun = args[1];
	toLower(&editNoun);
	if(editNoun ==STR_NAME)
	{
		string newname = join(2,args," ");
		dungeonExit->setPrimaryName(newname);
	}
	else if(editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		string desc = join(2,args," ");
		vector<string> descVector;
		descVector.push_back(desc);
		dungeonExit->description = descVector;
	}
	else if(editNoun == STR_DOOR)
	{
		dungeonExit->isDoor = isAffirmative(args[2]);
	}
	else if(editNoun == STR_OPEN)
	{
		dungeonExit->isOpen = isAffirmative(args[2]);
	} 
	else if(editNoun == STR_OPEN_TEXT)
	{
		dungeonExit->openText = join(2,args," ");
	}
	else if(editNoun == STR_CLOSED_TEXT)
	{
		dungeonExit->closedText = join(2,args," ");
	}

	else {
		return "I don't know how to set that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string ExitEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);

	if(addNoun == STR_NAME)
	{
		if(args.size() < 3)
		{
			return "Provide a name to add please.";
		}
		string name = join(2,args," ");
		dungeonExit->addName(name);		
	}	
	else
	{
		return "I don't know how to add that";
	}
	clearWindows();
	resetWindows();
	return "";
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
		return set(args);

	}
	else if (editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		TextEditor ed;		
		dungeonExit->description = ed.edit("Editing Description For Object:"+dungeonExit->getPrimaryName(),dungeonExit->description);
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
		newRoom->addName(join(2,args," "));
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

	string command;

	int lineCount = 3;
	setcolors(mainWindow,1,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"Exit Editor");
	setcolor(mainWindow,2,COLOR_WHITE);


	string nameRow = STR_MENU_NAME + join(0,dungeonExit->getNames(),",");
	mvwprintw(mainWindow,lineCount,0,nameRow.c_str());

	lineCount++;
	string desc = dungeonExit->description.size() > 0 ? dungeonExit->description[0] + STR_ELLIPSES : "";
	string descRow = STR_MENU_DESCRIPTION + desc;
	mvwprintw(mainWindow,lineCount,0,descRow.c_str());

	lineCount++;
	string roomRow = STR_MENU_ROOM + dungeonExit->room->getPrimaryName();
	mvwprintw(mainWindow,lineCount,0,roomRow.c_str());

	lineCount++;
	string torf = dungeonExit->isDoor ? STR_TRUE : STR_FALSE;
	string doorRow = STR_MENU_IS_DOOR + torf;
	mvwprintw(mainWindow,lineCount,0,doorRow.c_str());

	if (dungeonExit->isDoor) {
		lineCount++;
		torf = dungeonExit->isOpen ? STR_TRUE : STR_FALSE;
		string openRow = STR_MENU_IS_OPEN + torf;
		mvwprintw(mainWindow,lineCount,0,openRow.c_str());

		lineCount++;
		string openTextRow = STR_MENU_OPENTEXT + dungeonExit->openText;
		mvwprintw(mainWindow,lineCount,0,openTextRow.c_str());

		lineCount++;
		string closedTextRow = STR_MENU_CLOSEDTEXT + dungeonExit->closedText;
		mvwprintw(mainWindow,lineCount,0,closedTextRow.c_str());

		lineCount++;
		string openingTextRow = STR_MENU_OPENINGTEXT + dungeonExit->openingText;
		mvwprintw(mainWindow,lineCount,0,openingTextRow.c_str());

		lineCount++;
		string closingTextRow = STR_MENU_CLOSINGTEXT + dungeonExit->closingText;
		mvwprintw(mainWindow,lineCount,0,closingTextRow.c_str());

	}



	wrefresh(mainWindow);

}

DungeonRoom* ExitEditor::load(DungeonExit *_dungeonExit, DungeonRoom *_fromRoom)
{
	dungeonExit = _dungeonExit;
	fromRoom = _fromRoom;
	cmdMap[STR_EDIT] = &ExitEditor::edit;
	cmdMap[STR_EXIT] = &ExitEditor::exit;
	cmdMap[STR_CREATE] = &ExitEditor::create;
	cmdMap[STR_SET] = &ExitEditor::set;
	cmdMap[STR_ADD] = &ExitEditor::add;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;

    while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
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
}






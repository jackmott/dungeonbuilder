
#include "exit_editor.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include "dungeon_list.h"

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
	string setNoun = args[1];
	toLower(&setNoun);

	if(setNoun == STR_ROOM)
	{
		DungeonRoomList dl;
		DungeonRoom * newRoom = dl.load(g_roomList,dungeonExit);
		dungeonExit->room = newRoom;

	}
	else if(setNoun ==STR_NAME)
	{
		string newname = join(2,args," ");
		dungeonExit->setPrimaryName(newname);
	}
	else if(setNoun == STR_DOOR)
	{
		dungeonExit->isDoor = isAffirmative(args[2]);
	}
	else if(setNoun == STR_OPEN)
	{
		dungeonExit->isOpen = isAffirmative(args[2]);
	}
	else if(setNoun == STR_OPEN_TEXT || setNoun == STR_DESC)
	{
		dungeonExit->openText = join(2,args," ");
	}
	else if(setNoun == STR_CLOSED_TEXT)
	{
		dungeonExit->closedText = join(2,args," ");
	}
	else if(setNoun == STR_CLOSING_TEXT)
	{
		dungeonExit->closingText = join(2,args," ");
	}
	else if(setNoun == STR_OPENING_TEXT)
	{
		dungeonExit->openingText = join(2,args," ");
	}



	else {
		return "I don't know how to set that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string ExitEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);

	if(delNoun == STR_NAME)
	{
		if(args.size() < 3)
		{
			return "Provide a name to delete.";
		}
		string name = join(2,args," ");
		if(!dungeonExit->removeName(name)) {
			return "You can't.";
		}

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
	else if(editNoun == STR_ROOM)
	{
		return STR_ROOM;
	}
	else
	{
		return "I don't know how to edit that";
	}

}


void ExitEditor::clearWindows()
{
	delwin(headerWindow);
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void ExitEditor::resetWindows()
{
	commandWindow = newwin(1,getCols(),LINES-1,0);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	headerWindow = newwin(1,getCols(),0,0);
	mainWindow = newwin(LINES-3,getCols()-2,0,1);
	
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	string command;

	
	
	string fromTxt = dungeonExit->fromRoom->getPrimaryName();
	
	string exitTxt =  STR_RIGHT_ARROW;
	exitTxt += "EXIT:" + dungeonExit->getPrimaryName()  + STR_RIGHT_ARROW;
	string toTxt = dungeonExit->room->getPrimaryName();

	int w = getCols();
	int startX = (w- (fromTxt.size()+exitTxt.size()+toTxt.size()))/2;
	init_pair(1,COLOR_BLACK,COLOR_RED);
	wbkgd(headerWindow,COLOR_PAIR(1));
	wclrtoeol(headerWindow);
	
	setcolors(headerWindow,2,COLOR_BLACK,COLOR_RED);	
	mvwprintw(headerWindow,0,startX,fromTxt.c_str());
	startX += fromTxt.size();
	setcolors(headerWindow,3,COLOR_WHITE,COLOR_RED);
	mvwprintw(headerWindow,0,startX,exitTxt.c_str());
	startX += exitTxt.size();
	setcolors(headerWindow,2,COLOR_BLACK,COLOR_RED);
	mvwprintw(headerWindow,0,startX,toTxt.c_str());
	wrefresh(headerWindow);

	int lineCount = 2;
	setcolor(mainWindow,4,COLOR_WHITE);


	string nameRow = STR_MENU_NAME + join(0,dungeonExit->getNames(),",");
	mvwprintw(mainWindow,lineCount,0,nameRow.c_str());

	lineCount++;
	string openTextRow = STR_MENU_OPENTEXT + dungeonExit->openText;
	mvwprintw(mainWindow,lineCount,0,openTextRow.c_str());

	lineCount++;
	string roomRow = STR_MENU_EXIT_ROOM + dungeonExit->room->getPrimaryName();
	mvwprintw(mainWindow,lineCount,0,roomRow.c_str());

	lineCount++;
	string torf = dungeonExit->isDoor ? STR_TRUE : STR_FALSE;
	string doorRow = STR_MENU_IS_DOOR + torf;
	mvwprintw(mainWindow,lineCount,0,doorRow.c_str());

	if(dungeonExit->isDoor) {
		lineCount++;
		torf = dungeonExit->isOpen ? STR_TRUE : STR_FALSE;
		string openRow = STR_MENU_IS_OPEN + torf;
		mvwprintw(mainWindow,lineCount,0,openRow.c_str());

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

DungeonRoom* ExitEditor::load(DungeonExit *_dungeonExit,DungeonRoom *_fromRoom)
{
	dungeonExit = _dungeonExit;
	fromRoom = _fromRoom;
	cmdMap[STR_EDIT] = &ExitEditor::edit;
	cmdMap[STR_EXIT] = &ExitEditor::exit;
	cmdMap[STR_SET] = &ExitEditor::set;
	cmdMap[STR_ADD] = &ExitEditor::add;
	cmdMap[STR_DELETE] = &ExitEditor::del;


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
				return nullptr;
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






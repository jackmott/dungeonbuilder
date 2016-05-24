
#include "creature_editor.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"


using namespace std;

string CreatureEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string CreatureEditor::set(vector<string> args)
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
		creature->name = newname;
	}
	else if(editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		string desc = join(2,args," ");
		vector<string> descVector;
		descVector.push_back(desc);
		creature->description = descVector;
	}
	else if(editNoun == STR_HITPOINTS)
	{
		string hitstring = args[2];
		int hitpoints = stoi(hitstring,nullptr,10);
		creature->hitpoints = hitpoints;		
	}
	else {
		return "I don't know how to set that";
	}
	clearWindows();
	resetWindows();
	return "";
}


string CreatureEditor::edit(vector<string> args)
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
		creature->description = ed.edit("Editing Description For Creature:"+creature->name,creature->description);
		clearWindows();
		resetWindows();
		return "";
	}
	else
	{
		return "I don't know how to edit that";
	}



}

string CreatureEditor::create(vector<string> args)
{
	return "Create not implemented";
}


void CreatureEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void CreatureEditor::resetWindows()
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
	mvwprintwCenterBold(mainWindow,1,"Creature Editor");
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = STR_MENU_NAME + creature->name;
	mvwprintw(mainWindow,3,0,nameRow.c_str());
	string desc = creature->description.size() > 0 ? creature->description[0] + STR_ELLIPSES : "";
	string descRow = STR_MENU_DESCRIPTION + desc;
	mvwprintw(mainWindow,4,0,descRow.c_str());
	string hitpointsRow = STR_MENU_HITPOINTS + to_string(creature->hitpoints);
	mvwprintw(mainWindow,5,0,hitpointsRow.c_str());

	wrefresh(mainWindow);

}

void CreatureEditor::load(DungeonCreature *_creature)
{
	creature = _creature;
	cmdMap[STR_EDIT] = &CreatureEditor::edit;
	cmdMap[STR_EXIT] = &CreatureEditor::exit;
	cmdMap[STR_CREATE] = &CreatureEditor::create;
	cmdMap[STR_SET] = &CreatureEditor::set;

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


	clearWindows();
}






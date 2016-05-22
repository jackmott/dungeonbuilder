
#include "headers/creature_editor.h"

using namespace std;

string CreatureEditor::exit(vector<string> args)
{
	return "exit";
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
	if(editNoun == "name")
	{
		string newname = join(2,args," ");
		creature->name = newname;
		clearWindows();
		resetWindows();
		return "";
	}
	else if(editNoun == "desc" || editNoun == "description")
	{
		string desc = join(2,args," ");
		creature->description = desc;
		clearWindows();
		resetWindows();
		return "";
	}
	else if(editNoun == "hitpoints")
	{
		string hitstring = args[2];
		int hitpoints = stoi(hitstring,nullptr,10);
		creature->hitpoints = hitpoints;
		clearWindows();
		resetWindows();
		return "";

	}
	else {
		return "I don't know how to set that";
	}
}

string CreatureEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == "name")
	{
		return set(args);

	}
	else if(editNoun == "description")
	{
		TextEditor ed;
		string newdesc = ed.edit("Editing Description For Creature:"+creature->name,creature->description);
		creature->description = newdesc;
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
	string nameRow = "[Name]" + creature->name;
	mvwprintw(mainWindow,3,0,nameRow.c_str());
	string descRow = "[Description] " + creature->description;
	mvwprintw(mainWindow,4,0,descRow.c_str());
	string hitpointsRow = "[Hitpoints] " + to_string(creature->hitpoints);
	mvwprintw(mainWindow,5,0,hitpointsRow.c_str());

	wrefresh(mainWindow);

}

void CreatureEditor::load(DungeonCreature *_creature)
{
	creature = _creature;
	cmdMap["edit"] = &CreatureEditor::edit;
	cmdMap["exit"] = &CreatureEditor::exit;
	cmdMap["create"] = &CreatureEditor::create;
	cmdMap["set"] = &CreatureEditor::set;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.command(commandWindow,":");
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
			if(cmd[0] == "exit") break;
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






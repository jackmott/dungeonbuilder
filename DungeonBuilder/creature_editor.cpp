#include "creature_editor.h"
#include "dungeon_creature.h"
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
		creature->setPrimaryName(newname);
	}
	else if(editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		string desc = join(2,args," ");		
		creature->description = desc;
	}
	else if(editNoun == STR_HITPOINTS)
	{
		string hitstring = args[2];
		creature->hitpoints = stoi(hitstring,nullptr,10);		 
	}
	else if(editNoun == STR_ALIGNMENT)
	{
		string alignstring = args[2];
		creature->alignment = stoi(alignstring,nullptr,10);		
	}
	else {
		return "I don't know how to set that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string CreatureEditor::del(vector<string> args)
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
		if(!creature->removeName(name)) {
			return "You can't.";
		}

	}
	clearWindows();
	resetWindows();
	return "";
}

string CreatureEditor::add(vector<string> args)
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
		creature->addName(name);
	}
	else
	{
		return "I don't know how to add that";
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
		creature->description = ed.edit("Editing Description For Creature:"+creature->getPrimaryName(),creature->description);
		clearWindows();
		resetWindows();
		return "";
	}
	else
	{
		return "I don't know how to edit that";
	}
}


void CreatureEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}

void CreatureEditor::resetWindows()
{
	commandWindow = newwin(1,getCols(),LINES-1,0);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	mainWindow = newwin(LINES-3,getCols(),1,0);
	headerWindow = newwin(1,getCols(),0,0);
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	string command;

	printHeader(headerWindow,creature->parent->getPrimaryName(),"CREATURE:"+creature->getPrimaryName(),"");
	
	setcolor(mainWindow,COLOR_WHITE);
	string nameRow = STR_MENU_NAME + join(0,creature->getNames(),",");
	mvwprintw(mainWindow,3,0,nameRow.c_str());
	string desc = creature->description.size() > 0 ? creature->description[0] + STR_ELLIPSES : "";
	string descRow = STR_MENU_DESCRIPTION + desc;
	mvwprintw(mainWindow,4,0,descRow.c_str());
	string hitpointsRow = STR_MENU_HITPOINTS + to_string(creature->hitpoints);
	mvwprintw(mainWindow,5,0,hitpointsRow.c_str());
	string alignmentRow = STR_MENU_ALIGNMENT + to_string(creature->alignment);
	mvwprintw(mainWindow,6,0,alignmentRow.c_str());

	wrefresh(mainWindow);

}

void CreatureEditor::load(DungeonCreature *_creature)
{
	creature = _creature;
	cmdMap[STR_EDIT] = &CreatureEditor::edit;
	cmdMap[STR_EXIT] = &CreatureEditor::exit;
	cmdMap[STR_ADD] = &CreatureEditor::add;
	cmdMap[STR_SET] = &CreatureEditor::set;
	cmdMap[STR_DELETE] = &CreatureEditor::del;
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






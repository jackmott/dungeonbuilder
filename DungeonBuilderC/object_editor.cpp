
#include "object_editor.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;

string ObjectEditor::exit(vector<string> args)
{
	return STR_EXIT;
}


string ObjectEditor::set(vector<string> args)
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
	if(editNoun == STR_NAME)
	{
		string newname = join(2,args," ");
		object->name = newname;
	}
	else if(editNoun == STR_DESCRIPTION || editNoun == STR_DESC)
	{
		string desc = join(2,args," ");
		vector<string> descVector;
		descVector.push_back(desc);
		object->description = descVector;
	}
	else if(editNoun == STR_DAMAGE)
	{
		string damagestring = args[2];
		int dmg = stoi(damagestring,nullptr,10);
		object->damage = dmg;
	}
	else if(editNoun == STR_TAKEABLE)
	{				
		object->canTake = isAffirmative(args[2]);		
	}
	else if(editNoun == STR_OPENABLE)
	{
			object->canOpen = isAffirmative(args[2]);		
	}
	else if(editNoun == STR_OPEN)
	{
			object->isOpen = isAffirmative(args[2]);		
	}
	else {
		return "I don't know how to set that";
	}

	clearWindows();
	resetWindows();
	return "";
}


string ObjectEditor::edit(vector<string> args)
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
		object->description = ed.edit("Editing Description For Object:"+object->name,object->description);
	}
	else
	{
		return "I don't know how to edit that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string ObjectEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);

	if(addNoun == STR_USE_ALIAS) {

		//TODO - check if use alias is already used for summin else
		if(args.size() < 3)
		{
			return "Provide a string to alias the verb 'use'";
		}
		string alias = args[2];
		object->useAliases.push_back(alias);
		clearWindows();
		resetWindows();
		return "";
	}
	
	else
	{
		return "I don't know how to add that";
	}
}

string ObjectEditor::create(vector<string> args)
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


	if(createNoun == STR_OBJECT)
	{
		ObjectEditor oe;
		DungeonObject* o = new DungeonObject();
		o->name = join(2,args," ");
		clearWindows();
		oe.load(o);
		object->contents.push_back(o);
		resetWindows();
		return "";
	}

	return "I don't know how to create that";
}


void ObjectEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void ObjectEditor::resetWindows()
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

	int lineCount = 1;
	setcolors(mainWindow,lineCount,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"Object Editor");

	lineCount++;
	lineCount++;
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = STR_MENU_NAME + object->name;
	mvwprintw(mainWindow,lineCount,0,nameRow.c_str());

	lineCount++;
	string desc = object->description.size() > 0 ? object->description[0] + STR_ELLIPSES : "";
	string descRow = STR_MENU_DESCRIPTION + desc;
	mvwprintw(mainWindow,lineCount,0,descRow.c_str());

	
	lineCount++;
	mvwprintw(mainWindow,lineCount,0,STR_MENU_OBJECT);
	for(auto o : object->contents)
	{
		lineCount++;
		string row = o->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	
	lineCount++;
	string aliasRow = STR_MENU_USE_ALIAS;
	aliasRow += join(0,object->useAliases,STR_JOINER);
	mvwprintw(mainWindow,lineCount,0,aliasRow.c_str());

	lineCount++;
	string dmgRow = STR_MENU_DAMAGE + to_string(object->damage);
	mvwprintw(mainWindow,lineCount,0,dmgRow.c_str());

	lineCount++;
	string torf = object->canTake? STR_TRUE : STR_FALSE;
	string takeRow = STR_MENU_TAKEABLE + torf;
	mvwprintw(mainWindow,lineCount,0,takeRow.c_str());

	lineCount++;
	torf = object->canOpen ? STR_TRUE : STR_FALSE;
	string canOpenRow = STR_MENU_CAN_OPEN + torf;
	mvwprintw(mainWindow,lineCount,0,canOpenRow.c_str());

	lineCount++;
	torf = object->isOpen ? STR_TRUE : STR_FALSE;
	string isOpenRow = STR_MENU_IS_OPEN + torf;
	mvwprintw(mainWindow,lineCount,0,isOpenRow.c_str());



	wrefresh(mainWindow);

}

void ObjectEditor::load(DungeonObject *_object)
{
	object = _object;
	cmdMap[STR_EDIT] = &ObjectEditor::edit;
	cmdMap[STR_EXIT] = &ObjectEditor::exit;
	cmdMap[STR_CREATE] = &ObjectEditor::create;
	cmdMap[STR_ADD] = &ObjectEditor::add;
	cmdMap[STR_SET] = &ObjectEditor::set;
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






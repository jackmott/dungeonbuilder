
#include "effect_editor.h"
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

string EffectEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string EffectEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);
	resetWindows();
	return "";
}

string EffectEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}
	if(args.size() < 3) {
		return "Please supply the value directly in the command.";
	}
	string editNoun = args[1];
	toLower(&editNoun);
		resetWindows();
	return "";
}


string EffectEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	
	clearWindows();
	resetWindows();
	return "";
}

string EffectEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);


	clearWindows();
	resetWindows();
	return "";
}


void EffectEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}

void EffectEditor::resetWindows()
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
	wrefresh(headerWindow);

	string command;

	
	/*printHeader(headerWindow,effect->parent->getPrimaryName(),"OBJECT:"+object->getPrimaryName());
	
	int lineCount = 2;	
	
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = STR_MENU_NAME + join(0,object->getNames(),",");
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
		string row = o->getPrimaryName();
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}


	lineCount++;	
	mvwprintw(mainWindow,lineCount,0,STR_MENU_ACTIONS);
	for(auto a : object->actions)
	{
		lineCount++;
		string row = a->getPrimaryName();
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}


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
	*/

}

void EffectEditor::load(DungeonEffect* _effect)
{
	effect = _effect;
	cmdMap[STR_EDIT] = &EffectEditor::edit;
	cmdMap[STR_EXIT] = &EffectEditor::exit;
	cmdMap[STR_ADD] = &EffectEditor::add;
	cmdMap[STR_SET] = &EffectEditor::set;
	cmdMap[STR_DELETE] = &EffectEditor::del;
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






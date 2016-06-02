
#include "action_editor.h"
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;

string ActionEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string ActionEditor::del(vector<string> args)
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

string ActionEditor::set(vector<string> args)
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


string ActionEditor::edit(vector<string> args)
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

string ActionEditor::add(vector<string> args)
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


void ActionEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}


void ActionEditor::resetWindows()
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

	
	printHeader(headerWindow,action->parent->parent->getPrimaryName(),action->parent->getPrimaryName(),"ACTION:"+action->getPrimaryName(),3);
	
	int lineCount = 2;	
	
	setcolor(mainWindow,2,COLOR_WHITE);
	
	wrefresh(mainWindow);
	
}

void ActionEditor::load(DungeonAction* _action)
{
	action = _action;
	cmdMap[STR_EDIT] = &ActionEditor::edit;
	cmdMap[STR_EXIT] = &ActionEditor::exit;
	cmdMap[STR_ADD] = &ActionEditor::add;
	cmdMap[STR_SET] = &ActionEditor::set;
	cmdMap[STR_DELETE] = &ActionEditor::del;
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






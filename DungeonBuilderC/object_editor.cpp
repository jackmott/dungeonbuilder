
#include "headers/object_editor.h"

using namespace std;

string ObjectEditor::exit(vector<string> args)
{
	return "exit";
}

string ObjectEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == "name")
	{
		//set it directly or go to editor
		if(args.size() < 3) {
			return "Please supply the name directly in the command";
		}
		else
		{
			string newname = join(2,args," ");
			object->name = newname;
			clearWindows();
			resetWindows();
			return "";
		}

	}
	else if(editNoun == "description")
	{
		TextEditor ed;
		string newdesc = ed.edit("Editing Description For Object:"+object->name,object->description);
		object->description = newdesc;
		clearWindows();
		resetWindows();
		return "";
	}
	else
	{
		return "I don't know how to edit that";
	}

}

string ObjectEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);

	if(addNoun == "usealias") {
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
	return "Create not implemented";
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

	setcolors(mainWindow,1,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"Object Editor");
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = "[Name]: " + object->name;
	mvwprintw(mainWindow,3,0,nameRow.c_str());
	string descRow = "[Description]: " + object->description;
	mvwprintw(mainWindow,4,0,descRow.c_str());
	string aliasRow = "[Add/Remove](UseAlias): ";
	aliasRow += join(0,object->useAliases,",");
	mvwprintw(mainWindow,5,0,aliasRow.c_str());
	wrefresh(mainWindow);

}

void ObjectEditor::load(DungeonObject *_object)
{
	object = _object;
	cmdMap["edit"] = &ObjectEditor::edit;
	cmdMap["exit"] = &ObjectEditor::exit;
	cmdMap["create"] = &ObjectEditor::create;
	cmdMap["add"] = &ObjectEditor::add;

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






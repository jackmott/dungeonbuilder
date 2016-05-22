
#include "headers/exit_editor.h"

using namespace std;

string ExitEditor::exit(vector<string> args)
{
	return "exit";
}

string ExitEditor::edit(vector<string> args)
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
		DungeonEditor ed;
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

string ExitEditor::create(vector<string> args)
{
	return "Create not implemented";
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
	getmaxyx(stdscr,h,w); //why the fuck doesn't this work?
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	int done = false;
	string command;

	setcolors(mainWindow,1,COLOR_RED,COLOR_BLACK);
	mvwprintwCenter(mainWindow,1,"Object Editor");
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = "[Name]" + object->name;
	mvwprintw(mainWindow,3,0,nameRow.c_str());
	string descRow = "[Description] " + object->description.substr(0,min(MAX_EDITOR_PRINT_WIDTH,(int)object->description.length()));
	if (MAX_EDITOR_PRINT_WIDTH < object->description.length()) descRow += "...";
	mvwprintw(mainWindow,4,0,descRow.c_str());

	wrefresh(mainWindow);

}

void ExitEditor::load(DungeonObject *_object)
{
	object = _object;
	cmdMap["edit"] = &ExitEditor::edit;
	cmdMap["exit"] = &ExitEditor::exit;
	cmdMap["create"] = &ExitEditor::create;

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
			if (cmd[0] == "exit") break;
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






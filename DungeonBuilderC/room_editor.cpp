
#include "headers/room_editor.h"

using namespace std;

string RoomEditor::exit(vector<string> args)
{
	return "exit";
}

string RoomEditor::edit(vector<string> args)
{
	//DungeonEditor ed;
	//ed.edit(room.name,room.description);
	return "test";
}


void RoomEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void RoomEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS,0,0);
	getmaxyx(stdscr,h,w); //why the fuck doesn't this work?
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	int done = false;
	string command;

	setcolor(mainWindow,1,COLOR_WHITE);
	string nameRow = "[Name]" + room.name;
	mvwprintwCenter(mainWindow,2,nameRow);
	string descRow = "[Description] " + room.description.substr(0,min(30,(int)room.description.length()));
	mvwprintwCenter(mainWindow,3,descRow.c_str());

	wrefresh(mainWindow);

}

void RoomEditor::load(DungeonRoom _room)
{
	room = _room;
	cmdMap["edit"] = &RoomEditor::edit;
	cmdMap["exit"] = &RoomEditor::exit;

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






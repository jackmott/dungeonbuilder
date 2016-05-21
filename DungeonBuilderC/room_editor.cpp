
#include "headers/room_editor.h"

using namespace std;

void RoomEditor::exit(vector<string> args)
{

}

void RoomEditor::edit(vector<string> args)
{
	DungeonEditor ed;
	ed.edit("Empty Room","Empty Room");
}


void RoomEditor::reset()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void RoomEditor::load(DungeonRoom room)
{
	cmdMap["edit"] = &RoomEditor::edit;
	cmdMap["exit"] = &RoomEditor::exit;

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
	string descRow = "[Description] " + room.description;
	mvwprintwCenter(mainWindow,3,descRow.c_str());	
	
	wrefresh(mainWindow);
	

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while (!cmdFound) {
		cmd = cmdW.command(commandWindow,":");
		if (cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;		
		}
		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
	}

	commandFunction cmdFunc = cmdMap[cmd[0]];
	(this->*cmdFunc)(cmd);
		
	


	reset();
}






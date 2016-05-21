#include "headers/main_menu.h"

using namespace std;

void MainMenu::exitMenu(vector<string> args)
{
	exit(EXIT_SUCCESS);
}

void MainMenu::create(vector<string> args)
{
	RoomEditor ed;
	DungeonRoom room;
	room.name = "Default Name";
	room.description = "Default Description";
	clearWindows();
	ed.load(room);
	resetWindows();
}


void MainMenu::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void MainMenu::resetWindows()
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

	setcolor(mainWindow,1,COLOR_RED);
	mvwprintwCenter(mainWindow,3,"Dungeon Builder");
	setcolor(mainWindow,2,COLOR_CYAN);
	mvwprintwCenter(mainWindow,5,"[Create] a new Dungeon");
	mvwprintwCenter(mainWindow,6,"[Exit] this world");
	wrefresh(mainWindow);


}

void MainMenu::load()
{
	cmdMap["create"] = &MainMenu::create;
	cmdMap["exit"] = &MainMenu::exitMenu;

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
		else {
			commandFunction cmdFunc = cmdMap[cmd[0]];
			(this->*cmdFunc)(cmd);
		}
	}



	clearWindows();
}






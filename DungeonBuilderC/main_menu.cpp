#include "main_menu.h"
#include <sstream>
#include <fstream>
#include "printutils.h"
#include "command_window.h"
#include "utils.h"
#include "room_editor.h"
#include "dungeon_engine.h"
#include "string_constants.h"



using namespace std;

DungeonRoom *g_startRoom = nullptr;
DungeonPlayer *g_player;
vector<DungeonRoom*> g_roomList;
unsigned long global_id = 0;



void MainMenu::exitMenu(vector<string> args)
{
	exit(EXIT_SUCCESS);
}

void MainMenu::create(vector<string> args)
{
	//Creating a new game
	//Create a room editor
	//Create a start room and set the global start room pointer
	//Add the room to the global room list
	//Edit the room
	RoomEditor ed;
	if (g_startRoom == nullptr)
	{
		g_startRoom = new DungeonRoom();
		g_startRoom->setPrimaryName("Start Room");		
	}		
	clearWindows();
	ed.load(g_startRoom);
	resetWindows();
}

void MainMenu::play(vector<string> args)
{
	DungeonEngine engine;
	g_player = new DungeonPlayer();
	engine.load(g_startRoom,g_player);
	resetWindows();
}

void MainMenu::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void MainMenu::saveMap(vector<string> args) 
{
	if (args.size() < 2)
		mvwprintw(responseWindow, 0, 0, "Save to what filename?");
	else if(!g_startRoom)
		mvwprintw(responseWindow, 0, 0, "Must make a room first");
	else 
	{
		ofstream fout((args[1].append(".json")).c_str());
		fout << "{" << endl;
		fout << STR_TAB << "\"rooms\": [" << endl;
		for (auto i = 0u; i < g_roomList.size(); i++) {
			fout << STR_TAB << STR_TAB << "{" << endl;
			fout << g_roomList[i]->toJSON();
			fout << STR_TAB << STR_TAB << "}," << endl;
		}
		fout << STR_TAB << "]" << endl;
		fout << "}";

		fout.close();
		mvwprintw(responseWindow, 0, 0, "File written");
	}
	wclrtoeol(responseWindow);
	wrefresh(responseWindow);
}

void MainMenu::loadMap(vector<string> args)
{
	if (args.size() != 2) 
	{
		mvwprintw(responseWindow, 0, 0, "Requires single filename to load");
	}
	else
	{
		JSONLoader loader(args[1].c_str());
		if (!loader.open)
		{
			mvwprintw(responseWindow, 0, 0, "Could not open that file");
		}
		else
		{
			g_roomList = loader.loadMap();
			g_startRoom = g_roomList[0];
		}
	}

	wclrtoeol(responseWindow);
	wrefresh(responseWindow);
}

void MainMenu::resetWindows()
{
	clear();
	commandWindow = newwin(1,getCols(),LINES-1,0);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	mainWindow = newwin(LINES-2,getCols(),0,0);
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	string command;

	setcolor(mainWindow,1,COLOR_RED);
	mvwprintwCenterBold(mainWindow,3,"Dungeon Builder <-> Pre Alpha");
	setcolor(mainWindow,2,COLOR_CYAN);

	mvwprintwBold(mainWindow,5,0,"[Create] a Dungeon");
	mvwprintwBold(mainWindow,6,0,"[Enter] Dungeon");
	mvwprintwBold(mainWindow,7,0,"[Save] this map");
	mvwprintwBold(mainWindow,8,0,"[Load] a map");
	mvwprintwBold(mainWindow,9,0,"[Exit] this world");
	wrefresh(mainWindow);
}

void MainMenu::load()
{
	cmdMap[STR_CREATE] = &MainMenu::create;
	cmdMap[STR_EXIT] = &MainMenu::exitMenu;
	cmdMap[STR_ENTER] = &MainMenu::play;
	cmdMap[STR_SAVE] = &MainMenu::saveMap;
	cmdMap[STR_LOAD] = &MainMenu::loadMap;

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
		else {
			commandFunction cmdFunc = cmdMap[cmd[0]];
			(this->*cmdFunc)(cmd);
		}
	}
}






#include "dungeon_effect.h"
#include "dungeon_trigger.h"
#include "dungeon_room.h"
#include "dungeon_creature.h"
#include "dungeon_exit.h"
#include "dungeon_object.h"
#include "dungeon_action.h"
#include "dungeon_player.h"
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
vector<DungeonObject*> g_objectList;
vector<DungeonCreature*> g_creatureList;
vector<DungeonExit*> g_exitList;
vector<DungeonAction*> g_actionList;
vector<DungeonEffect*> g_effectList;
vector<DungeonTrigger*> g_triggerList;


unsigned long global_id = 0;

void MainMenu::resize(vector<string> args)
{
	resize_term(0,0);
	resetWindows();
}
void MainMenu::exitMenu(vector<string> args)
{
	endwin();
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
	if(g_startRoom == nullptr)
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
	if(args.size() < 2)
		mvwprintw(responseWindow,0,0,"Save to what filename?");
	else if(!g_startRoom)
		mvwprintw(responseWindow,0,0,"Must make a room first");
	else
	{
		ofstream fout((args[1].append(".json")).c_str());

		fout << "{" << endl;

		fout << "\"rooms\":[" << endl;
		for(auto room : g_roomList) {
			fout << "{" << room->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"objects\":[" << endl;
		for(auto item : g_objectList) {
			fout << "{" << item->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"creatures\":[" << endl;
		for(auto creature : g_creatureList) {
			fout << "{" << creature->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"exits\":[" << endl;
		for(auto exit : g_exitList) {
			fout << "{" << exit->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"effects\":[" << endl;
		for(auto effect : g_effectList) {
			fout << "{" << effect->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"triggers\":[" << endl;
		for(auto trigger : g_triggerList) {
			fout << "{" << trigger->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"actions\":[" << endl;
		for(auto action : g_actionList) {
			fout << "{" << action->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "}";

		fout.close();
		mvwprintw(responseWindow,0,0,"File written");
	}
	wclrtoeol(responseWindow);
	wrefresh(responseWindow);
}

void MainMenu::loadMap(vector<string> args)
{
	if(args.size() != 2)
	{
		mvwprintw(responseWindow,0,0,"Requires single filename to load");
	}
	else
	{
		JSONLoader loader(args[1].c_str());
		if(!loader.open)
		{
			mvwprintw(responseWindow,0,0,"Could not open that file");
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
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS,0,0);
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	string command;

	setcolor(mainWindow,COLOR_RED);
	mvwprintwCenterBold(mainWindow,3,"Dungeon Builder <-> Pre Alpha");
	setcolor(mainWindow,COLOR_CYAN);

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
	cmdMap[STR_KEY_RESIZE] = &MainMenu::resize;

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





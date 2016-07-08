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

GlobalState globalState;


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
	if(globalState.startRoom == nullptr)
	{
		globalState.startRoom = new DungeonRoom();
		globalState.startRoom->setPrimaryName("Start Room");
	}
	clearWindows();
	ed.load(globalState.startRoom);
	resetWindows();
}

void MainMenu::play(vector<string> args)
{
	DungeonEngine engine;
	globalState.player = new DungeonPlayer();
	engine.load(globalState.startRoom,globalState.player);
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
	else if(!globalState.startRoom)
		mvwprintw(responseWindow,0,0,"Must make a room first");
	else
	{
		ofstream fout((args[1].append(".json")).c_str());

		fout << "{" << endl;

		fout << "\"rooms\":[" << endl;
		for(auto room : globalState.roomList) {
			fout << "{" << room->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"objects\":[" << endl;
		for(auto item : globalState.objectList) {
			fout << "{" << item->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"creatures\":[" << endl;
		for(auto creature : globalState.creatureList) {
			fout << "{" << creature->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"exits\":[" << endl;
		for(auto exit : globalState.exitList) {
			fout << "{" << exit->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"effects\":[" << endl;
		for(auto effect : globalState.effectList) {
			fout << "{" << effect->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"triggers\":[" << endl;
		for(auto trigger : globalState.triggerList) {
			fout << "{" << trigger->toJSON() << "}," << endl;
		}
		fout << "]," << endl;

		fout << "\"actions\":[" << endl;
		for(auto action : globalState.actionList) {
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
		loadJson(args[1]);		
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
	
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	
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
		else if (cmd.size() > 0) {
			commandFunction cmdFunc = cmdMap[cmd[0]];
			(this->*cmdFunc)(cmd);
		}
	}
}






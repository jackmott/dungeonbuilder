#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include "printutils.h"
#include "command_window.h"
#include "utils.h"
#include "dungeon_data.h"
#include "room_editor.h"
#include "dungeon_engine.h"

using namespace std;


struct MainMenu
{
	typedef void (MainMenu::*commandFunction) (vector<string>);
	map<string,commandFunction> cmdMap;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	void create(vector<string> args);
	void play(vector<string> args);
	void exitMenu(vector<string> args);
	void clearWindows();
	void saveMap(vector<string> args);
	void loadMap(vector<string> args);
	void resetWindows();
	void load();
	
};

#endif
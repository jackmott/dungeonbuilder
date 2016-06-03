#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <vector>
#include <string>
#include <map>
#include "lib.h"
#include "JSON_loader.h"




struct MainMenu
{
	typedef void (MainMenu::*commandFunction) (std::vector<std::string>);
	std::map<std::string,commandFunction> cmdMap;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	void create(std::vector<std::string> args);
	void play(std::vector<std::string> args);
	void exitMenu(std::vector<std::string> args);
	void clearWindows();
	void saveMap(std::vector<std::string> args);
	void loadMap(std::vector<std::string> args);
	void resetWindows();
	void load();
	
};

#endif
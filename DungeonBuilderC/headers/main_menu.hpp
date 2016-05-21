#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>
#include "parser.hpp"



struct mainMenu
{
		
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	
	void reset();

	void load();
				
};

#endif
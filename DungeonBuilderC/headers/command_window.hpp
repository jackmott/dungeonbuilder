#pragma once
#ifndef COMMAND_WINDOW
#define COMMAND_WINDOW

#include "curses.h"
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

struct CommandWindow
{
	WINDOW *window;
	int x;	
	int w;
	string input;
	int done = 0;
	string prompt;
				
	void reset();	
	string command(WINDOW* _window,string prompt);				
	void moveLeft();
	void moveRight();			
	bool handleInput(int c);
	void print();
	

};


#endif
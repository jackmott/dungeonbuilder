#pragma once
#ifndef COMMAND_WINDOW
#define COMMAND_WINDOW

#include "curses.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
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
	vector<string> command(WINDOW* _window,string prompt);				
	void moveLeft();
	void moveRight();			
	bool handleInput(int c);
	void print();
	

};


#endif
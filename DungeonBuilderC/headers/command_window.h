#pragma once
#ifndef COMMAND_WINDOW
#define COMMAND_WINDOW

#include <string>
#include <vector>
#include "lib.h"

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
	vector<string> getCommand(WINDOW* _window,string prompt);				
	string getCommandAsString(WINDOW* _window,string prompt);				
	void moveLeft();
	void moveRight();			
	bool handleInput(int c);
	void print();
    void backspace();
};


#endif
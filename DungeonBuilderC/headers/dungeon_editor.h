#pragma once
#ifndef DUNGEON_EDITOR
#define DUNGEON_EDITOR

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>
#include "headers/printutils.h"

using namespace std;

struct DungeonEditor
{

	int x,y;	
	vector<string> lines;
	int done = 0;
	string header;
	WINDOW *headerWindow;
	WINDOW *mainWindow;

	int w,h;

	
	void reset();
	
	string edit(string _header, string startText);
		
	string remTabs(string line);

	void insertLine(string line,int n);

	void appendLine(string line);

	void removeLine(int n);

	void moveLeft();

	void moveRight();
	void moveUp(); 
	void moveDown();

	void deleteLine();
	void deleteLine(int n);

		
	void handleInput(int c);
	void printBuff();
	void printStatusLine();

};


#endif
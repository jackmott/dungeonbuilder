#pragma once
#ifndef TEXT_EDITOR
#define TEXT_EDITOR

#ifdef _WIN32
#include "pdcurses.h"
#else
#include <ncurses.h>
#endif
#include <vector>
#include <string>

using namespace std;

struct TextEditor
{

	unsigned int x,y;	
	vector<string> lines;
	int done = 0;
	string header;
	WINDOW *headerWindow;
	WINDOW *mainWindow;

	unsigned int w;

	
	void clearWindows();
	
	vector<string> edit(string _header, vector<string> startText);
		
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
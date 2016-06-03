#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <vector>
#include <string>
#include "lib.h"

struct TextEditor
{
	unsigned int x,y;	
	std::vector<std::string> lines;
	int done = 0;

	WINDOW *headerWindow;
	WINDOW *mainWindow;

	unsigned int w;
	
	void clearWindows();
	
	std::vector<std::string> edit(std::string _header, std::vector<std::string> startText);
		
	std::string remTabs(std::string line);

	void insertLine(std::string line,int n);

	void appendLine(std::string line);

	void removeLine(int n);

	void moveLeft();

	void moveRight();
	void moveUp(); 
	void moveDown();

	void deleteLine();
	void deleteLine(int n);

		
	void handleInput(int c);
	void printBuff();

};


#endif
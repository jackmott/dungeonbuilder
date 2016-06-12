#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <vector>
#include <string>
#include "lib.h"

struct TextEditor
{

	size_t x,y,pos;
	std::string text;
	int done = 0;

	WINDOW *headerWindow;
	WINDOW *mainWindow;

	unsigned int w;
	
	void clearWindows();
	
	std::vector<size_t> lineLengths;
	std::string edit(std::string _header, std::string startText);
		
	std::string remTabs(std::string line);

	

	void moveLeft();
	void moveRight();
	void moveUp(); 
	void moveDown();
	
		
		
	void handleInput(int c);
	void printBuff();

};


#endif
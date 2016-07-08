#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <string>
#include <vector>
#include "lib.h"

struct CommandWindow
{
	WINDOW *window;
	size_t x;	
	int w;
	std::string input;
	bool done = false;
	std::string prompt;
				
	void reset();	
	std::vector<std::string> commandBuffer;
	size_t cmdBufferPos = 0;
	std::vector<std::string> getCommand(WINDOW* _window,std::string prompt);				
	std::string getCommandAsString(WINDOW* _window,std::string prompt);				
	void moveLeft();
	void moveRight();			
	bool handleInput(int c);
	void print();
};


#endif
#include "command_window.h"
#include <algorithm>
#include <sstream>
#include "utils.h"
#include "printutils.h"
#include "string_constants.h"

using namespace std;

void CommandWindow::reset(){
	wclear(window);
}

string CommandWindow::getCommandAsString(WINDOW* _window,string _prompt) {
	input = "";
	prompt = _prompt;
	window = _window;
	keypad(window,true);
	x = prompt.length();
	done = false;

	w = getmaxx(window); //This doesn't update in Windows

	while(!done)
	{
		print();
		int c = wgetch(window);
		done = handleInput(c);
	}
	reset();

	if(input != STR_PAGE_UP && (input != STR_PAGE_DOWN))
	{
		commandBuffer.push_back(input);
		cmdBufferPos = commandBuffer.size();
	}

	return input;
}

vector<string> CommandWindow::getCommand(WINDOW* _window,string _prompt) {
	string input = getCommandAsString(_window,_prompt);
	vector<string> result = split(input,' ');
	return result;
}

void CommandWindow::moveLeft() {
}

void CommandWindow::moveRight() {
}

bool CommandWindow::handleInput(int c) {


	switch(c)
	{
	case KEY_HOME:
		x = 0+prompt.size();
		break;
	case KEY_END:
		x = input.size()+prompt.size();
		break;
	case KEY_BACKSPACE:
	case 127: // Mac OSX delete key
	case 8:  //backspace
		if(x > prompt.size())
		{
			x = x -1;
			input.erase(x-prompt.size(),1);
		}
		break;
	case KEY_DC:
		if(input.size() - x+prompt.size() > 0)
			input.erase(x-prompt.size(),1);
		break;
	case KEY_UP:
		if(cmdBufferPos > 0)
		{
			cmdBufferPos--;
			input = commandBuffer[cmdBufferPos];
			x = input.size()+prompt.size();
		}

		break;
	case KEY_DOWN:
		if(cmdBufferPos+1 < commandBuffer.size())
		{
			cmdBufferPos++;
			input = commandBuffer[cmdBufferPos];
			x = input.size()+prompt.size();
		}
		break;
	case KEY_LEFT:
		if(x > prompt.size())
		{
			x--;
		}
		break;
	case KEY_RIGHT:
		if(x < input.size()+prompt.size())
		{
			x++;
		}
		break;
	case KEY_PPAGE: //page up
		input = STR_PAGE_UP;
		return true;
		break;
	case KEY_NPAGE: //page down
		input = STR_PAGE_DOWN;
		return true;
		break;
	case KEY_RESIZE:
		input = STR_KEY_RESIZE;
		return true;
		break;
	case KEY_ENTER:
	case 10:
		return true;
	case 27:  //escape key
		input = STR_EXIT;;
		return true;
	default:
		if(input.size() < (unsigned int)(COLS -2))
		{
			input.insert(x-prompt.size(),1,(char)c);
			x++;
		}
	}

	return false;
}

void CommandWindow::print(){
	string output = prompt + input;
	mvwprintw(window,0,0,output.c_str());
	wclrtoeol(window);
	wmove(window,0,x);
}





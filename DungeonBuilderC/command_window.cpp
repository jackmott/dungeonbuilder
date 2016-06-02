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
    commandBuffer.push_back(input);
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
	case KEY_BACKSPACE:
    case 127: // Mac OSX delete key
	case 8:  //backspace
		if(input.length() > 0)
		{ 
			input.pop_back();
            x--;
		}
		break;
	case KEY_UP:
		if (commandBuffer.size() > 0)
		{
			input = commandBuffer[commandBuffer.size()-1];
			commandBuffer.pop_back();
			x = input.size();
		}
		
		break;
	case KEY_LEFT:
		input = STR_PREV;
		return true;
		break;
	case KEY_RIGHT:
		input = STR_NEXT;
		return true;
		break;
	case KEY_PPAGE: //page up
		input = STR_PAGE_UP;
		return true;
		break;
	case KEY_NPAGE: //page down
		input = STR_PAGE_DOWN;
		return true;
		break;
	case KEY_ENTER:
	case 10:
		return true;
	case 27:  //escape key
		input = STR_EXIT;;
		return true;
	default:
		if(input.size() < (unsigned int)(getCols() -2))
		{
			input.push_back((char)c);
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





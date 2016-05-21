#include "headers/command_window.hpp"

using namespace std;

void CommandWindow::reset(){

}

string CommandWindow::command(WINDOW* _window,string _prompt) {
	
	prompt = _prompt;
	window = _window;
	x = prompt.length();
	done = false;
				
	w = getmaxx(window); //why the fuck doesn't this work?
		
	while(!done)
	{
		print();
		int input = wgetch(window);
		done = handleInput(input);
	}
	
	return input;
	
}
void CommandWindow::moveLeft() {
}

void CommandWindow::moveRight() {

}
bool CommandWindow::handleInput(int c){
	switch(c)
	{
		case KEY_BACKSPACE:
		case 8:  //backspace
			if (input.length() > 0)
				input.pop_back();
				x--;
			break;
		case KEY_ENTER:
		case 10:
			return true;
		default:
			input.push_back((char)c);
			x++;
	}
	return false;

}

void CommandWindow::print(){
	string output = prompt + input;
	mvwprintw(window,0,0,output.c_str());	
	wclrtoeol(window);	
	wmove(window,0,x);
}
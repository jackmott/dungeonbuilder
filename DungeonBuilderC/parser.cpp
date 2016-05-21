#include "headers/parser.hpp"

using namespace std;

bool parseCommand(int c,string *buffer)
{
	switch(c)
	{
		case KEY_BACKSPACE:
		case 8:  //backspace
			buffer->pop_back();
			break;
		case KEY_ENTER:
		case 10:
			return true;
		default:
			buffer->push_back((char)c);
	}
	return false;
}


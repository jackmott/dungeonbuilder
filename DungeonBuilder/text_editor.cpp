#include "text_editor.h"
#include "string_constants.h"
#include "printutils.h"
#include "utils.h"
using namespace std;


void TextEditor::clearWindows()
{
	x = 0;
	y = 0;
	done = 0;
	delwin(headerWindow);
	delwin(mainWindow);
	clear();
}

vector<string> TextEditor::edit(string header,vector<string> startText)
{
	text = join(0,startText," ");
	x = 0;
	y = 0;
	pos = 0;

	headerWindow = newwin(1,getCols(),0,0);
	mainWindow = newwin(LINES-1,getCols(),1,0);

	printHeader(headerWindow,header);

	keypad(mainWindow,true);   //turns on arrows and f keys

	wrefresh(headerWindow);
	wrefresh(mainWindow);

	while(!done)
	{
		printBuff();
		int input = wgetch(mainWindow);
		handleInput(input);
	}

	clearWindows();
	return split(text,CHR_SPACE);
}


string TextEditor::remTabs(string line) {
	int tab = line.find("\t");
	if(tab == line.npos)
		return line;
	else
		return remTabs(line.replace(tab,1,STR_TAB));
}


void TextEditor::moveLeft() {
	if (pos > 0) pos--;
}

void TextEditor::moveRight() {
	if (pos < text.size()) pos++;
}

void TextEditor::moveUp() {
	if(y > 0)
		y--;
	wmove(mainWindow,y,x);
}
void TextEditor::moveDown() {
	y++;
	wmove(mainWindow,y,x);
}



void TextEditor::handleInput(int c) {
	switch(c)
	{
	case KEY_HOME:
		x = 0;
		break;
	case KEY_END:
		x = getCols();
		break;
	case KEY_LEFT:
		moveLeft();
		break;
	case KEY_RIGHT:
		moveRight();
		break;
	case KEY_UP:
		moveUp();
		break;
	case KEY_DOWN:
		moveDown();
		break;
	case 27: //escape key
		done = 1;
		break;
	case KEY_BACKSPACE:
	case 127: // Mac OSX delete key
	case 8:  // backspace
		// The Backspace key
		if(pos != 0)
		{
			pos--;
			text.erase(pos,1);
		}
		break;
	case KEY_DC:
		if(pos < text.size())
		{
			text.erase(pos,1);
			pos--;
		}
		break;
	case KEY_ENTER:
	case 10:
		text.insert(pos,1,char(CHR_NEWLINE));
		pos++;
		break;
	case KEY_BTAB:
	case KEY_CTAB:
	case KEY_STAB:
	case KEY_CATAB:
	case 9:
		// The Tab key
		text.insert(pos,4,CHR_SPACE);
		pos += 4;
		break;
	default:
		// Any other character insert
		text.insert(pos,1,char(c));
		pos++;
		break;
	}
}

void TextEditor::printBuff() {
	wclear(mainWindow);
	lineLengths.clear();
	size_t px=0;
	size_t py = 0;
	x=0;
	y=0;
	vector<string> tokens = splitOnSpaceAndEnter(text);
	size_t counter = 0;
	for(auto token : tokens) {
		int remainingWidth = getCols() - px;
		if(token[0] == CHR_SPACE)
		{
			for(size_t i = 0; i < token.size(); i++)
			{
				if(px >= getCols()){
					lineLengths.push_back(px);
					py++;
					px = 0;
				}
				mvwaddch(mainWindow,py,px,CHR_SPACE);
				if(counter == pos)
				{
					x = px;
					y = py;
				}
				counter++;
				px++;
			}
		}
		else
		{
			if(token.size() > remainingWidth ) {
				lineLengths.push_back(px);
				py++;
				px = 0;
			}
			for(size_t i = 0; i < token.size(); i++)
			{
				mvwprintw(mainWindow,py,px,&token[i]);			
				if(counter == pos)
				{
					x = px;
					y = py;
				}
				if(i == token.size()-1 && (unsigned char)token[i] == CHR_NEWLINE)
				{
					px = 0;
					py++;
				}
				else
				{
					px++;
				}								
				counter++;
			}
			
		}
	}
	if(counter == pos)
	{
		x = px;
		y = py;
	}
	wmove(mainWindow,y,x);
}



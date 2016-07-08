#include "text_editor.h"
#include "string_constants.h"
#include "printutils.h"
#include "utils.h"
using namespace std;

extern GlobalState globalState;

void TextEditor::clearWindows()
{
	x = 0;
	y = 0;
	done = 0;
	delwin(headerWindow);
	delwin(mainWindow);
	clear();
}

void TextEditor::resetWindows()
{
	headerWindow = newwin(1,COLS,0,0);
	mainWindow = newwin(LINES-1,COLS,1,0);
	printHeader(headerWindow,"Dungeon Edit",header);
	keypad(mainWindow,true);   //turns on arrows and f keys
}

string TextEditor::edit(const string &_header,const string &startText)
{
	text = startText;
	x = 0;
	y = 0;
	pos = 0;
	hStart = 0;
	hEnd = 0;
	header = _header;

	resetWindows();
	wrefresh(headerWindow);
	wrefresh(mainWindow);

	while(!done)
	{
		printBuff();
		int input = wgetch(mainWindow);
		handleInput(input);
	}

	clearWindows();
	return text;
}


string TextEditor::remTabs(string line) {
	int tab = line.find("\t");
	if(tab == line.npos)
		return line;
	else
		return remTabs(line.replace(tab,1,STR_TAB));
}


void TextEditor::moveLeft() {
	if(pos > 0) pos--;
}


// abcd 1234
// efgh 5431
void TextEditor::moveRight() {
	if(pos < text.size()) pos++;
}

void TextEditor::moveUp() {
	if(y > 0) {
		pos = pos - x; //pop us to the start of this line
		if(x < lineLengths[y-1])
		{
			pos = pos - (lineLengths[y-1] - x); //then to the proper position of the previous line
		}
		else
		{
			pos--;
		}
	}

}
void TextEditor::moveDown() {
	if(y < lineLengths.size()-1)
	{
		pos = pos + (lineLengths[y]-x);
		if(x < lineLengths[y+1])
		{
			pos = pos + x;
		}
		else {
			pos = pos + lineLengths[y+1]-1;
		}
	}
}



void TextEditor::handleInput(int c) {
	switch(c)
	{
	case KEY_RESIZE:
		resize_term(0,0);
		resetWindows();
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
		if(hStart != hEnd) {
			hStart = 0; hEnd = 0;
		}
		else {
			done = 1;
		}
		break;
	case KEY_BACKSPACE:
	case 127: // Mac OSX delete key
	case 8:  // backspace
		// The Backspace key
		if(hStart != hEnd) {
			text.erase(hStart,hEnd-hStart);
			pos = hStart;
			hStart = 0; hEnd = 0;
		}
		else {
			if(pos != 0)
			{
				pos--;
				text.erase(pos,1);
			}
		}
		break;
	case KEY_DC:
		if(hStart != hEnd) {
			text.erase(hStart,hEnd-hStart);
			pos = hStart;
			hStart = 0; hEnd = 0;
		}
		else {
			if(pos < text.size())
			{
				text.erase(pos,1);
			}
		}
		break;
	case KEY_ENTER:
	case 10:
		text.insert(pos,1,char(CHR_NEWLINE));
		pos++;
		break;
	case KEY_HOME:
		pos = pos - x;
		break;
	case KEY_END:
		pos = pos + lineLengths[y] - x ;
		break;
	case KEY_SHOME:
		if(pos > hEnd)
			hEnd = pos;
		pos = pos - x;
		hStart = pos;
		break;
	case KEY_SEND:
		if(hStart > pos || hStart == hEnd) {
			hStart = pos;
		}
		pos = pos + lineLengths[y]-x;
		hEnd = pos;
		break;
	case KEY_SLEFT:
		if(pos == hStart)
		{
			pos--;
			hStart = pos;
		}
		else if(pos == hEnd) {
			pos--;
			hEnd = pos;
		}
		else {
			hEnd = pos;
			pos--;
			hStart = pos;
		}
		break;
	case KEY_SRIGHT:
		if(pos == hEnd) {
			pos++;
			hEnd = pos;
		}
		else if(pos == hStart)
		{
			pos++;
			hStart++;
		}
		else {
			hStart = pos;
			pos++;
			hEnd = pos;
		}
		break;
	case KEY_SUP:
		if(pos == hStart)
		{
			moveUp();
			hStart = pos;
		}
		else if(pos == hEnd)
		{
			moveUp();
			hEnd = pos;
		}
		else {
			hEnd = pos;
			moveUp();
			hStart = pos;
		}
		break;
	case KEY_SDOWN:
		if(pos == hEnd)
		{
			moveDown();
			hEnd = pos;
		}
		else if(pos == hStart)
		{
			moveDown();
			hStart = pos;
		}
		else {
			hStart = pos;
			moveDown();
			hEnd = pos;
		}
		break;
	case 3: //CNTRL C IN WINDOWS
		globalState.clipboard = text.substr(hStart,hEnd-hStart);
		hStart = 0; hEnd = 0;
		break;
	case 22: //CNTRL V IN WINDOWS
		if(hStart != hEnd) {
			text.erase(hStart,hEnd-hStart);
			pos = hStart;
			hStart = 0; hEnd = 0;
		}
		text.insert(pos,globalState.clipboard);

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

		if(hEnd != hStart)
		{
			pos = hStart;
			text.erase(hStart,hEnd-hStart);
			hEnd = 0;
			hStart = 0;
		}
		text.insert(pos,1,char(c));
		pos++;
		break;
	}
}

void TextEditor::printBuff() {
	if(is_term_resized(LINES,COLS))
	{
		resize_term(0,0);
	}
	wclear(mainWindow);
	lineLengths.clear();
	size_t px=0;
	size_t py = 0;
	x=0;
	y=0;
	vector<string> tokens = splitOnSpaceAndEnter(text);
	size_t counter = 0;
	for(auto token : tokens) {
		int remainingWidth = COLS - px;
		if(token[0] == CHR_SPACE)
		{
			for(size_t i = 0; i < token.size(); i++)
			{
				if(px >= (size_t)COLS){
					lineLengths.push_back(px);
					py++;
					px = 0;
				}
				if(counter >= hStart && counter < hEnd)
				{
					setcolors(mainWindow,COLOR_BLACK,COLOR_WHITE);
				}
				else
				{
					setcolors(mainWindow,COLOR_WHITE,COLOR_BLACK);
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
			if((int)token.size() > remainingWidth) {
				lineLengths.push_back(px);
				py++;
				px = 0;
			}
			for(size_t i = 0; i < token.size(); i++)
			{
				if(counter >= hStart && counter < hEnd)
				{
					setcolors(mainWindow,COLOR_BLACK,COLOR_WHITE);
				}
				else
				{
					setcolors(mainWindow,COLOR_WHITE,COLOR_BLACK);
				}

				mvwprintw(mainWindow,py,px,&token[i]);
				if(counter == pos)
				{
					x = px;
					y = py;
				}
				if(i == token.size()-1 && (unsigned char)token[i] == CHR_NEWLINE)
				{
					lineLengths.push_back(px+1);
					py++;
					px = 0;
				}
				else
				{
					px++;
				}
				counter++;
			}

		}
	}
	lineLengths.push_back(px);
	if(counter == pos)
	{
		x = px;
		y = py;
	}
	wmove(mainWindow,y,x);
}



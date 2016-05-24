#include "text_editor.h"

using namespace std;


void TextEditor::clearWindows()
{
	x = 0;
	y = 0;
	done = 0;
	header = "";	
	delwin(headerWindow);
	delwin(mainWindow);	
	clear();
}

vector<string> TextEditor::edit(string _header,vector<string> startText)
{	
	header = _header;
	lines = startText;
	x = 0;
	y = 0;
	appendLine("");
	headerWindow = newwin(1,COLS,0,0);
	mainWindow = newwin(LINES-1,COLS,1,0);
	scrollok(mainWindow,true);
	keypad(mainWindow,true);   //turns on arrows and f keys
	w = getmaxx(stdscr); // this doesn't work in windows

	refresh();
	wrefresh(headerWindow);
	wrefresh(mainWindow);
	
	while(!done)
	{
		printBuff();
		printStatusLine();
		wrefresh(headerWindow);
		int input = wgetch(mainWindow);
		handleInput(input);
	}
	
	clearWindows();
	return lines;
}


string TextEditor::remTabs(string line) {
	int tab = line.find("\t");
	if(tab == line.npos)
		return line;
	else
		return remTabs(line.replace(tab,1,"    "));
}

void TextEditor::insertLine(string line,int n) {
	line = remTabs(line);
	lines.insert(lines.begin()+n,line);
}

void TextEditor::appendLine(string line) {
	line = remTabs(line);
	lines.push_back(line);
}

void TextEditor::removeLine(int n) {
	lines.erase(lines.begin()+n);
}

void TextEditor::moveLeft() {
	if(x-1 >= 0)
	{
		x--;
		wmove(mainWindow,y,x);
	}
}
void TextEditor::moveRight() {
	if(x+y < w && x+1 <= lines[y].length())
	{
		x++;
		wmove(mainWindow,y,x);
	}
}
void TextEditor::moveUp() {
	if(y-1 >= 0)
		y--;
	if(x >= lines[y].length())
		x = lines[y].length();
	wmove(mainWindow,y,x);
}
void TextEditor::moveDown() {

	if(y+1 < lines.size())
		y++;
	if(x >= lines[y].length())
		x = lines[y].length();
	wmove(mainWindow,y,x);
}


void TextEditor::deleteLine() {
	removeLine(y);
}
void TextEditor::deleteLine(int n) {
	removeLine(n);
}


void TextEditor::handleInput(int c) {
	switch(c)
	{
	case KEY_LEFT:
		moveLeft();
		break;
	case KEY_RIGHT:
		moveRight();
		break;
	case KEY_UP:
		//moveUp();
		scroll(mainWindow);
		break;
	case KEY_DOWN:
		moveDown();
		break;
	case 27: //escape key
		done = 1;
		break;
	case 127:
	case KEY_BACKSPACE:
	case 8:  //KEY_BACKSPACE:
		// The Backspace key
		if(x == 0 && y > 0)
		{
			x = lines[y-1].length();
			// Bring the line down
			lines[y-1] += lines[y];
			// Delete the current line
			deleteLine();
			moveUp();
		}
		else
		{
			// Removes a character
			if(x>0 || y>0)
				lines[y].erase(--x,1);

		}
		break;
	case KEY_DC:
		// The Delete key
		if(x == lines[y].length() && y != lines.size() - 1)
		{
			// Bring the line down
			lines[y] += lines[y+1];
			// Delete the line
			deleteLine(y+1);
		}
		else
		{
			lines[y].erase(x,1);
		}
		break;
	case KEY_ENTER:
	case 10:
		// The Enter key
		// Bring the rest of the line down
		if(x < lines[y].length())
		{
			// Put the rest of the line on a new line
			insertLine(lines[y].substr(x,lines[y].length() - x),y + 1);
			// Remove that part of the line
			lines[y].erase(x,lines[y].length() - x);
		}
		else
		{
			insertLine("",y+1);
		}
		x = 0;
		moveDown();
		break;
	case KEY_BTAB:
	case KEY_CTAB:
	case KEY_STAB:
	case KEY_CATAB:
	case 9:
		// The Tab key
		lines[y].insert(x,4,' ');
		x += 4;
		break;
	default:
		// Any other character insert
		if(x < (unsigned int)COLS) {
			lines[y].insert(x,1,char(c));
			x++;
		}
		else {
			//nope
		}
		

		break;
	}
}
void TextEditor::printBuff() {
	for(auto i = 0u; i < lines.size(); i++)
	{
		if(i >= lines.size())
		{
			wmove(mainWindow,i,0);
			wclrtoeol(mainWindow);
		}
		else
		{
			mvwprintw(mainWindow,i,0,lines[i].c_str());
		}
		wclrtoeol(mainWindow);
	}
	int maxY = getmaxy(mainWindow);
    if (y > (unsigned int)maxY-1) {
		scroll(mainWindow);
	}
	wmove(mainWindow,y,x);
	
}
void TextEditor::printStatusLine() {


	setcolors(headerWindow,3,COLOR_BLACK,COLOR_RED);
	wclear(headerWindow);
	mvwprintwCenterBold(headerWindow,0,header);
	wclrtoeol(headerWindow);

}


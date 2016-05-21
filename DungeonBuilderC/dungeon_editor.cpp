#include "headers/dungeon_editor.hpp"
#include "headers/printutlls.hpp"
using namespace std;


void DungeonEditor::reset()
{
	x = 0;
	y = 0;
	done = 0;
	header = "";
	lines.clear();
	delwin(headerWindow);
	delwin(mainWindow);
}

string DungeonEditor::edit(string _header,string startText)
{
	header = _header;
	x = 0;
	y = 0;
	appendLine("");
	headerWindow = newwin(1,COLS,0,0);
	mainWindow = newwin(LINES-1,COLS,1,0);
	getmaxyx(stdscr,h,w); //why the fuck doesn't this work?

	refresh();
	wrefresh(headerWindow);
	wrefresh(mainWindow);


	for(string::size_type i = 0; i < startText.length();i++)
	{
		handleInput((int)startText[i]);
	}

	while(!done)
	{


		printBuff();
		printStatusLine();

		wrefresh(headerWindow);
		int input = wgetch(mainWindow);
		handleInput(input);
	}
	string result;
	for(int i = 0; i < lines.size()-1; i++)
	{
		result = result + lines[i];
	}
	reset();
	return result;
}


string DungeonEditor::remTabs(string line) {
	int tab = line.find("\t");
	if(tab == line.npos)
		return line;
	else
		return remTabs(line.replace(tab,1,"    "));
}

void DungeonEditor::insertLine(string line,int n) {
	line = remTabs(line);
	lines.insert(lines.begin()+n,line);
}

void DungeonEditor::appendLine(string line) {
	line = remTabs(line);
	lines.push_back(line);
}

void DungeonEditor::removeLine(int n) {
	lines.erase(lines.begin()+n);
}

void DungeonEditor::moveLeft() {
	if(x-1 >= 0)
	{
		x--;
		wmove(mainWindow,y,x);
	}
}
void DungeonEditor::moveRight() {
	if(x+y < w && x+1 <= lines[y].length())
	{
		x++;
		wmove(mainWindow,y,x);
	}
}
void DungeonEditor::moveUp() {
	if(y-1 >= 0)
		y--;
	if(x >= lines[y].length())
		x = lines[y].length();
	wmove(mainWindow,y,x);
}
void DungeonEditor::moveDown() {

	if(y+1 < h-1 && y+1 < lines.size())
		y++;
	if(x >= lines[y].length())
		x = lines[y].length();
	wmove(mainWindow,y,x);
}


void DungeonEditor::deleteLine() {
	removeLine(y);
}
void DungeonEditor::deleteLine(int n) {
	removeLine(n);
}


void DungeonEditor::handleInput(int c) {
	switch(c)
	{
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
	case 27:
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
		// Any other character
		lines[y].insert(x,1,char(c));
		x++;
		break;
	}
}
void DungeonEditor::printBuff() {
	for(int i = 0; i < h-1; i++)
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
	wmove(mainWindow,y,x);
}
void DungeonEditor::printStatusLine() {

	init_pair(1,COLOR_BLACK,COLOR_RED);
	wattron(headerWindow,COLOR_PAIR(1));
	wclear(headerWindow);
	mvwprintwCenter(headerWindow,0,header);
	wclrtoeol(headerWindow);
	wattroff(headerWindow,COLOR_PAIR(1));
}


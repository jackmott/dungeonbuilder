#include "curses.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct editor
{

	int x,y;	
	vector<string> lines;
	int done = 0;

	editor() {
		x = 0;
		y = 0;				
		appendLine("");

	}
	editor(string s)  {
		x = 0;
		y = 0;
		
		istringstream iss(s);
		string line;
		while(getline(iss,line))
		{
			appendLine(line);
		}

	}

	string edit()
	{
		while(!done)
		{		
			printStatusLine();
			printBuff();
			int input = getch();
			handleInput(input);
		}
		string result;
		for(int i = 0; i < lines.size()-1; i++)
		{
			result = result + lines[i];
		}
		return result;
	}

	string remTabs(string line) {
		int tab = line.find("\t");
		if(tab == line.npos)
			return line;
		else
			return remTabs(line.replace(tab,1,"    "));
	}

	void insertLine(string line,int n) {
		line = remTabs(line);
		lines.insert(lines.begin()+n,line);
	}

	void appendLine(string line) {
		line = remTabs(line);
		lines.push_back(line);
	}

	void removeLine(int n) {
		lines.erase(lines.begin()+n);
	}

	void moveLeft() {
		if(x-1 >= 0)
		{
			x--;
			move(y,x);
		}
	}
	void moveRight() {
		if(x+y < COLS && x+1 <= lines[y].length())
		{
			x++;
			move(y,x);
		}
	}
	void moveUp() {
		if(y-1 >= 0)
			y--;
		if(x >= lines[y].length())
			x = lines[y].length();
		move(y,x);
	}
	void moveDown() {

		if(y+1 < LINES-1 && y+1 < lines.size())
			y++;
		if(x >= lines[y].length())
			x = lines[y].length();
		move(y,x);
	}


	void deleteLine() {
		removeLine(y);
	}
	void deleteLine(int n) {
		removeLine(n);
	}

		
	void handleInput(int c) {
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
				if (x>0 || y>0)
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
	void printBuff() {
		for(int i = 0; i < LINES-1; i++)
		{
			if(i >= lines.size())
			{
				move(i,0);
				clrtoeol();
			}
			else
			{
				mvprintw(i,0,lines[i].c_str());
			}
			clrtoeol();
		}
		move(y,x);
	}
	void printStatusLine() {
		init_pair(1,COLOR_RED,COLOR_BLACK);
		attron(COLOR_PAIR(1));
		attron(A_REVERSE);
		mvprintw(LINES-1,0,"Dungeon Room Editor");
		clrtoeol();
		attroff(A_REVERSE);
		attroff(COLOR_PAIR(1));
	}

};

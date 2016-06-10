#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include <map>

using namespace std;


size_t getRows()
{
#ifdef TODO_MAYBE
	//Why doesn't this work?
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	unsigned int rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	return rows;
#else
	return (size_t)LINES;
#endif // _WIN32

}

size_t getCols()
{
#ifdef TODO_MAYBE
	//Why doesn't this work?
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	unsigned int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	return columns;
#else
	return (size_t)COLS;
#endif

}

void mvwprintwCenter (WINDOW * window,int row,string text)
{
	int w = getmaxx(window);
	mvwprintw(window,row,(w-text.length())/2,text.c_str());
}


void mvwprintwBold (WINDOW *window,int row,int col,string text)
{
	wattron(window,A_BOLD);
	mvwprintw(window,row,col,text.c_str());
	wattroff(window,A_BOLD);
}

void mvwprintwCenterBold (WINDOW *window,int row,string text)
{
	wattron(window,A_BOLD);
	mvwprintw(window,row,(getCols()-text.length())/2,text.c_str());
	wattroff(window,A_BOLD);
}


#define DUNGEON_HEADER_BG COLOR_RED
#define DUNGEON_HEADER_FG COLOR_BLACK
#define DUNGEON_HEADER_FG_BOLD COLOR_WHITE

void printHeader(WINDOW *window,string leftText,string centerText,string rightText,int boldIndex)
{

	setbackground(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wclrtoeol(window);

	centerText = STR_RIGHT_ARROW + centerText + STR_RIGHT_ARROW;


	int startX = (getCols() - (leftText.size()+rightText.size()+centerText.size()))/2;

	if(boldIndex == 1)
	{
		setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
		mvwprintwBold(window,0,startX,leftText.c_str());
	}
	else
	{
		setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
		mvwprintw(window,0,startX,leftText.c_str());
	}
	startX += leftText.size();

	if(boldIndex == 2)
	{
		setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
		mvwprintwBold(window,0,startX,centerText.c_str());
	}
	else
	{
		setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
		mvwprintw(window,0,startX,centerText.c_str());
	}
	startX += centerText.size();
	setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	if(boldIndex == 3)
	{
		setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
		mvwprintwBold(window,0,startX,rightText.c_str());
	}
	else
	{
		setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
		mvwprintw(window,0,startX,rightText.c_str());
	}
	wrefresh(window);
}

void printHeader(WINDOW *window,string leftText,string rightText){

	setbackground(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wclrtoeol(window);

	rightText = STR_RIGHT_ARROW + rightText;

	int startX = (getCols() - (leftText.size()+rightText.size()))/2;
	setcolors(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	mvwprintw(window,0,startX,leftText.c_str());
	startX += leftText.size();
	setcolors(window,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
	mvwprintwBold(window,0,startX,rightText.c_str());
	wrefresh(window);
}

void printHeader(WINDOW* window,string text)
{
	setbackground(window,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wclrtoeol(window);

	mvwprintwCenterBold(window,0,text.c_str());
	wrefresh(window);
}

//to keep track of color pairs and group numbers because
//ncurses is silly
map<int,int> colorMap;
int uGroup = 1;

int getColorGroup(int fore,int back)
{
	//25 bigger than any foreground color making hash unique for all color combos
	int colorHash = fore + back*25;
	auto findResult = colorMap.find(colorHash);
	int group = NULL;
	if(findResult == colorMap.end())
	{
		init_pair(uGroup,fore,back);
		colorMap.insert(make_pair(colorHash,uGroup));
		group = uGroup;
		uGroup++;
	}
	else
	{
		group = findResult->second;
	}
	return group;
}

void setcolor (WINDOW* window,int fore)
{
	setcolors(window,fore,COLOR_BLACK);
}
void setcolors(WINDOW *window,int fore,int back)
{
	int group = getColorGroup(fore,back);
	wattron(window,COLOR_PAIR(group));
}

void setbackground(WINDOW *window,int fore,int back)
{
	int group = getColorGroup(fore,back);
	wbkgd(window,COLOR_PAIR(group));
}


// #red#
vector<DungeonLine*>* parseDungeonText(vector<string> *text)
{
	chtype attributes = A_NORMAL;
	int backColor = COLOR_BLACK;
	int foreColor = COLOR_WHITE;
	vector<DungeonLine*> *result = new vector<DungeonLine*>();
	for(auto line : *text)
	{
		DungeonLine *dLine = new DungeonLine();
		for(size_t i = 0; i < line.size(); i++)
		{
			char c = line[i];
			if(c == '#')
			{
				size_t nextPound = line.find("#",i+1);
				if(nextPound == string::npos) continue; //ignore if no closing pound
				string oToken = line.substr(i+1,nextPound-(i+1));
				string token = toLower(oToken);
				i = i + token.size() + 1; //advance past the end of the # # token
				bool isColor = true;
				if(token == "black")
				{
					foreColor = COLOR_BLACK;
				}
				else if(token == "red")
				{
					foreColor = COLOR_RED;
				}
				else if(token == "green")
				{
					foreColor = COLOR_GREEN;
				}
				else if(token == "yellow")
				{
					foreColor = COLOR_YELLOW;
				}
				else if(token == "blue")
				{
					foreColor = COLOR_BLUE;
				}
				else if(token == "magenta")
				{
					foreColor = COLOR_MAGENTA;
				}
				else if(token == "cyan")
				{
					foreColor = COLOR_CYAN;
				}
				else if(token == "white")
				{
					foreColor = COLOR_WHITE;				
				}
				else {
					isColor = false;
				}

				if(isColor) {
					if(oToken[0] == toupper(token[0]))
					{
						attributes = A_BOLD;
					}
					else {
						attributes	= A_NORMAL;
					}
				}			
			}
			else
			{
				DungeonChar dc;
				dc.c = c;
				dc.attributes = attributes;
				dc.backColor = backColor;
				dc.foreColor = foreColor;
				dLine->chars->push_back(dc);
			}
		}
		result->push_back(dLine);
	}
	return result;
}

void renderDungeonText(WINDOW * window,vector<DungeonLine*> *text)
{
	int currentColor = COLOR_WHITE;
	int currentBgColor = COLOR_BLACK;
	setcolors(window,currentColor,currentBgColor);


	for(size_t y = 0; y < text->size(); y++)
	{
		DungeonLine* line = (*text)[y];
		for(size_t x = 0; x < line->chars->size(); x++)
		{
			DungeonChar c = (*line->chars)[x];
			if(c.foreColor != currentColor || c.backColor != currentBgColor)
			{
				setcolors(window,c.foreColor,c.backColor);
				currentColor = c.foreColor;
				currentBgColor = c.backColor;
			}
			mvwaddch(window,y,x,c.c | c.attributes);
		}
	}
}

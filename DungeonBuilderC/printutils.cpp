#include "printutils.h"
#include "string_constants.h"


using namespace std;



unsigned int getRows()
{
#ifdef TODO_MAYBE
	//Why doesn't this work?
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	unsigned int rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	return rows;
#else
	return LINES;
#endif // _WIN32

}

unsigned int getCols()
{
#ifdef TODO_MAYBE
	//Why doesn't this work?
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	unsigned int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	return columns;
#else
	return COLS;
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


void setcolor (WINDOW* window,int group,int fore)
{
	setcolors(window,group,fore,COLOR_BLACK);
}
void setcolors(WINDOW *window,int group,int fore,int back)
{
	init_pair(group,fore,back);
	wattron(window,COLOR_PAIR(group));
}


#define DUNGEON_HEADER_BG COLOR_RED
#define DUNGEON_HEADER_FG COLOR_BLACK
#define DUNGEON_HEADER_FG_BOLD COLOR_WHITE

void printHeader(WINDOW *window,string leftText,string centerText,string rightText)
{

	init_pair(10,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	wbkgd(window,COLOR_PAIR(10));
	wclrtoeol(window);

	
	centerText = STR_RIGHT_ARROW + centerText + STR_RIGHT_ARROW;
	

	int startX = (getCols() - (leftText.size()+rightText.size()+centerText.size()))/2;
	setcolors(window,11,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	mvwprintw(window,0,startX,leftText.c_str());
	startX += leftText.size();
	setcolors(window,12,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
	mvwprintwBold(window,0,startX,centerText.c_str());
	startX += centerText.size();
	setcolors(window,11,DUNGEON_HEADER_FG,DUNGEON_HEADER_BG);
	mvwprintw(window,0,startX,rightText.c_str());
	wrefresh(window);
}

void printHeader(WINDOW* window,string text)
{
	init_pair(10,DUNGEON_HEADER_FG_BOLD,DUNGEON_HEADER_BG);
	wbkgd(window,COLOR_PAIR(10));
	wclrtoeol(window);

	mvwprintwCenterBold(window,0,text.c_str());

}
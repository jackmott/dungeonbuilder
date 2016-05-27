#include "printutils.h"



using namespace std;



unsigned int getRows()
{
#ifdef TODO_MAYBE
	//Why doesn't this work?
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    unsigned int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);    
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

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;    

    return columns;
#else
	return COLS;
#endif
    
}

void mvwprintwCenter (WINDOW * window,int row, string text)
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
	int w = getmaxx(stdscr);
	mvwprintw(window,row,(w-text.length())/2,text.c_str());
	wattroff(window,A_BOLD);
}


void setcolor (WINDOW* window,int group, int fore)
{
	setcolors(window,group,fore,COLOR_BLACK);
}
void setcolors(WINDOW *window,int group,int fore,int back)
{	
	init_pair(group,fore,back);
	wattron(window,COLOR_PAIR(group));
}



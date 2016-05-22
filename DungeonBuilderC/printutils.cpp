#include "headers/printutils.h"

using namespace std;


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
	int w = getmaxx(window);
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



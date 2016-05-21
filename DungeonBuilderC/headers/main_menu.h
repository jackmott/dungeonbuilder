#pragma once

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct mainMenu
{
		
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	mainMenu() {
	
	}
	

	void reset()
	{		
		delwin(commandWindow);
		delwin(responseWindow);
		delwin(mainWindow);
	}

	void load()
	{		
				
		commandWindow = newwin(1,COLS,LINES-1,0);		
		responseWindow = newwin(1,COLS,LINES-2,0);
		mainWindow = newwin(LINES-2,COLS,0,0);
		getmaxyx(stdscr,h,w); //why the fuck doesn't this work?
				
		refresh();		
		wrefresh(commandWindow);								
		wrefresh(responseWindow);
		wrefresh(mainWindow);
		wmove(commandWindow,0,0);
		
	
		int done = false;
		while(!done)
		{				
																	
			int input = wgetch(commandWindow);									
			
		}
	
	}

		
	
};

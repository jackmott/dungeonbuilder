#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <string>
#include "lib.h"
#include <vector>


struct DungeonChar {
	char c;
	int color; 
	bool bold;
};

struct DungeonLine {
	std::vector<DungeonChar> *chars;
	bool centered = false;
	bool leftAlign = false;
	bool rightAlign = false;
	int indent = 0;
};

void renderDungeonText(WINDOW* window,std::vector<DungeonLine*> *text);


void mvwprintwCenter (WINDOW * window,int row, std::string  text);
void mvwprintwCenterBold (WINDOW * window,int row, std::string  text);
void mvwprintwBold (WINDOW * window,int row,int col, std::string  text);
void setcolor (WINDOW* window, int group, int fore);
void setcolors(WINDOW* window, int group, int fore,int back);
unsigned int getCols();
unsigned int getRows();
void printHeader(WINDOW *window,std::string leftText,std::string centerText,std::string rightText, int boldIndex = 2);
void printHeader(WINDOW* window,std::string text);
void printHeader(WINDOW *window,std::string leftText,std::string rightText);

#endif
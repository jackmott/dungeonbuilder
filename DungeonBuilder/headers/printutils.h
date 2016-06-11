#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <string>
#include "lib.h"
#include <vector>


struct DungeonChar {
	chtype c;
	int foreColor;
	int backColor;
	chtype attributes;
};

enum class TEXT_ALIGN {CENTER, LEFT, RIGHT};
struct DungeonLine {
	DungeonLine() {};
	std::vector<DungeonChar> *chars = new std::vector<DungeonChar>();
	TEXT_ALIGN alignment = TEXT_ALIGN::LEFT;
	int indent = 0;
};

std::vector<DungeonLine*> *parseDungeonText(std::vector<std::string> *text);
void renderDungeonText(WINDOW* window,std::vector<DungeonLine*> *text);


void mvwprintwCenter (WINDOW * window,int row, std::string  text);
void mvwprintwCenterBold (WINDOW * window,int row, std::string  text);
void mvwprintwBold (WINDOW * window,int row,int col, std::string  text);
int getColorGroup(int fore,int back);
void setbackground(WINDOW* window,int fore,int back);
void setcolor (WINDOW* window, int fore);
void setcolors(WINDOW* window, int fore,int back);
unsigned int getCols();
unsigned int getRows();
void printHeader(WINDOW *window,std::string leftText,std::string centerText,std::string rightText, int boldIndex = 2);
void printHeader(WINDOW* window,std::string text);
void printHeader(WINDOW *window,std::string leftText,std::string rightText);

#endif
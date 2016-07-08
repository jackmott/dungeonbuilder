#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <string>
#include "lib.h"
#include <vector>


enum class DUNGEON_ALIGN {
	LEFT,
	CENTER
};

struct DungeonChar {
	chtype c;
	int foreColor;
	int backColor;
	bool bold;
	DUNGEON_ALIGN alignment;
};

typedef std::vector<DungeonChar> DungeonToken;
typedef std::vector<DungeonToken> DungeonChunk;



std::vector<DungeonChunk> parseDungeonText(std::vector<std::string> &textBuffer);
void renderDungeonText(WINDOW* window,const std::vector<DungeonChunk> &chunks, int lineOffset);


void mvwprintwCenter (WINDOW * window,int row, const std::string  &text);
void mvwprintwCenterBold (WINDOW * window,int row, const std::string  &text);
void mvwprintwBold (WINDOW * window,int row,int col, const std::string  &text);
int getColorGroup(int fore,int back);
void setbackground(WINDOW* window,int fore,int back);
void setcolor (WINDOW* window, int fore);
void setcolors(WINDOW* window, int fore,int back);
void printHeader(WINDOW *window,const std::string &heading,const std::string &leftText,const std::string &centerText,const std::string &rightText, int boldIndex = 2);
void printHeader(WINDOW* window,const std::string &heading,const std::string &text);
void printHeader(WINDOW *window,const std::string &heading,const std::string &leftText,const std::string &rightText);
void printEngineHeader(WINDOW *window,const std::string &heading,const std::string &location,const std::string &turns);
#endif
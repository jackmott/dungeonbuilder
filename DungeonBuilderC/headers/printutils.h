#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <string>
#include "lib.h"


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
#pragma once
#ifndef PRINT_UTILS
#define PRINT_UTILS

#include "curses.h"
#include <string>
using namespace std;

#define MAX_EDITOR_PRINT_WIDTH 60

void mvwprintwCenter (WINDOW * window,int row, string text);
void mvwprintwCenterBold (WINDOW * window,int row, string text);
void mvwprintwBold (WINDOW * window,int row,int col, string text);
void setcolor (WINDOW* window, int group, int fore);
void setcolors(WINDOW* window, int group, int fore,int back);



#endif
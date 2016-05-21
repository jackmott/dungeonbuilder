#pragma once
#ifndef PRINT_UTILS
#define PRINT_UTILS

#include "curses.h"
#include <string>
using namespace std;


void mvwprintwCenter (WINDOW * window,int row, string text);
void setcolor (WINDOW* window, int group, int fore);
void setcolors(WINDOW* window, int group, int fore,int back);



#endif
#pragma once
#ifndef PARSER
#define PARSER

#include <string>
#include "curses.h"




bool parseCommand(int c,std::string *buffer);

#endif
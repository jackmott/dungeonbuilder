#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;


vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
string join(int offset, vector<string> &v, string delim);
void toLower(string* s);

#endif

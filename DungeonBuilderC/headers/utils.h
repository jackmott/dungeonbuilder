#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <vector>

using namespace std;


vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
string join(unsigned int offset, vector<string> &v, string delim);
vector<string> removeArticles(vector<string> words);
bool containsWith(vector<string> word);
void toLower(string* s);
bool isVowel(char c);
string a_an (string s);
bool isAffirmative(string s);


#endif

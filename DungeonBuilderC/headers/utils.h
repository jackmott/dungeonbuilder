#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <vector>
#include "dungeon_data.h"

using namespace std;


vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
string join(unsigned int offset, vector<string> &v, string delim);
vector<string> removeArticles(vector<string> words);
bool containsWith(vector<string> word);
string toLower(string s);
void toLower(string *s);
bool isVowel(char c);
string a_an (string s);
bool isAffirmative(string s);
string thereIsA(string thing);
DungeonEntity* extractEntity(void* _entities ,string *userInput);
void removeObject(vector<DungeonObject*> *objects,DungeonObject *object);
void removeCreature(vector<DungeonCreature*> *creatures,DungeonCreature *creature);
void removeExit(vector<DungeonExit*> *exits,DungeonExit *e);
bool removeStr(vector<string> *strs,string str);
void strlensort(vector<string>* v);
void entitysort(vector<DungeonEntity*> *v);
string extractPhrase(vector<string> phrasesToFind, string *userInput);
void dbsleep(unsigned int milliseconds);

#endif

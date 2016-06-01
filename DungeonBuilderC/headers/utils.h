#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <vector>
#include "dungeon_data.h"

using namespace std;
extern unsigned long global_id;

unsigned long getUID();
vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
string join(unsigned int offset, vector<string> const &v, string delim);
vector<string> removeArticles(vector<string> words);
bool containsWith(vector<string> word);
string toLower(string s);
void toLower(string *s);
bool isVowel(char c);
string a_an (string s);
bool isAffirmative(string s);
string thereIsA(string thing);
DungeonEntity* extractEntity(void* _entities ,string *userInput, int *matchedName = nullptr);
DungeonObject* extractObject(vector<DungeonObject*> * objects,string *userInput, int* matchedName = nullptr);
DungeonObject* extractAndRemoveObject(vector<DungeonObject*> * objects,string *userInput);
void removePointer(void * _pointers,void* pointer);
bool removeStr(vector<string> *strs,string str);
void strlensort(vector<string>* v);
void entitysort(vector<DungeonEntity*> *v);
string extractPhrase(vector<string> phrasesToFind, string *userInput);
void dbsleep(unsigned int milliseconds);

#endif

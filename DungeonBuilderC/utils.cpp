#include "utils.h"
#include <sstream>
#include <algorithm>

vector<string> &split(const string &s,char delim,vector<string> &elems) {
	stringstream ss(s);
	string item;
	while(getline(ss,item,delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> removeArticles(vector<string> words)
{
	vector<string> result;
	for(auto word: words)
	{
		string lowerWord = word;
		toLower(&lowerWord);

		if(lowerWord != "a" && lowerWord != "the")
		{
			result.push_back(word);
		}
	}
	return result;
}

bool containsWith(vector<string> words)
{

	for(auto word: words)
	{
		string lowerWord = word;
		toLower(&lowerWord);
		if(lowerWord == "with") return true;
	}
	return false;
}


vector<string> split(const string &s,char delim) {
	vector<string> elems;
	split(s,delim,elems);
	return elems;
}

bool isVowel(char c)
{
	switch(c)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return true;
	default:
		return false;
	}

}

string a_an (string s)
{
	string lowerS = s;
	toLower(&lowerS);
	if(isVowel(lowerS[0]))
	{
		return "an "+s;
	}
	else {
		return "a "+s;
	}
}

string join(unsigned int offset,vector<string> &v,string delim)
{
	string result;
	for(auto i = offset; i < v.size();i++)
	{
		result = result + v[i];
		if(i < v.size()-1)
		{
			result = result + delim;
		}
	}
	return result;
}

string toLower(string s)
{
	transform(s.begin(),s.end(),s.begin(),::tolower);
	return s;
}

void toLower(string *s)
{
	transform(s->begin(),s->end(),s->begin(),::tolower);	
}


bool isAffirmative(string s)
{
	string l = s;
	toLower(&l);

	if (l == "t" || l=="y" || l=="yes" || l == "true") return true;
	return false;
}

string thereIsA(string thing)
{
	return "There is " + a_an(thing) + " here";
}


DungeonObject * extractObject(vector<DungeonObject*> *objects ,string *userInput)
{
	string lcaseInput = toLower(*userInput);
	for(auto o : *objects)
	{
		string lcaseName = toLower(o->name);
		size_t pos = lcaseInput.find(lcaseName);
		if(pos != string::npos)
		{
			userInput->erase(pos,o->name.length());
			return o;
		}
	}
	return nullptr;
}

DungeonCreature* extractCreature(vector<DungeonCreature*> *creatures ,string *userInput)
{
	string lcaseInput = toLower(*userInput);
	for(auto creature : *creatures)
	{
		string lcaseName = toLower(creature->name);
		size_t pos = lcaseInput.find(lcaseName);
		if(pos != string::npos)
		{
			userInput->erase(pos,creature->name.length());
			return creature;
		}
	}
	return nullptr;
}


DungeonObject * extractAndRemoveObject(vector<DungeonObject*> *objects ,string *userInput)
{
	string lcaseInput = toLower(*userInput);
	int i = 0;
	for(auto o : *objects)
	{
		string lcaseName = toLower(o->name);
		size_t pos = lcaseInput.find(lcaseName);
		if(pos != string::npos)
		{
			userInput->erase(pos,o->name.length());
			objects->erase(objects->begin()+i);
			return o;
		}
		i++;
	}
	return nullptr;
}

DungeonCreature* extractAndRemoveCreature(vector<DungeonCreature*> *creatures ,string *userInput)
{
	string lcaseInput = toLower(*userInput);
	int i = 0;
	for(auto creature : *creatures)
	{
		string lcaseName = toLower(creature->name);
		size_t pos = lcaseInput.find(lcaseName);
		if(pos != string::npos)
		{
			userInput->erase(pos,creature->name.length());
			creatures->erase(creatures->begin()+i);
			return creature;
		}
		i++;
	}
	return nullptr;
}

string extractPhrase(vector<string> *phrasesToFind, string *userInput)
{

	//Find if anything in phrasesToFind exist in userInput, case insensitive
	//Remove the matches from userInput
	//return the match
	string lcaseInput = toLower(*userInput);
	for(auto phrase : *phrasesToFind)
	{
		string lcasePhrase = toLower(phrase);
		size_t pos = lcaseInput.find(lcasePhrase);
		if(pos != string::npos)
		{
			userInput->erase(pos,phrase.length());
			return phrase;
		}
	}
	return "";
}
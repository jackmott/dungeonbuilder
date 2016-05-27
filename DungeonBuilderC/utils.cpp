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



struct strlencmp {
	bool operator()(const string&first,const string& second)
	{
		return first.size() > second.size();
	}
};

void strlensort(vector<string> *v)
{
	strlencmp scmp;
	sort(v->begin(),v->end(),scmp);
}


struct entitycmp {
	bool operator()(const DungeonEntity*first,const DungeonEntity* second)
	{
		return first->name.size() > second->name.size();
	}
};


void entitysort(vector<DungeonEntity*> *v)
{
	entitycmp ccmp;
	sort(v->begin(),v->end(),ccmp);
}



//sort strings from longest to shortest


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



void removeObject(vector<DungeonObject*> *objects,DungeonObject *object)
{
	int i = 0;
	for(auto o : *objects)
	{
		if(o == object)
		{
			objects->erase(objects->begin()+i);
			return;
		}
		i++;
	}

}

void removeCreature(vector<DungeonCreature*> *creatures, DungeonCreature *creature)
{
	int i = 0;
	for(auto c : *creatures)
	{
		if(c == creature)
		{
			creatures->erase(creatures->begin()+i);
			return;
		}
		i++;
	}
}


// void* considered harmful. living dangerously.
DungeonEntity* extractEntity(void * _entities ,string *userInput)
{
	vector<DungeonEntity*> *entities = (vector<DungeonEntity*> *)_entities;
	entitysort(entities);
	string lcaseInput = toLower(*userInput);
	for(auto e : *entities)
	{
		size_t pos = lcaseInput.find(e->lcasename);
		if(pos != string::npos)
		{
			userInput->erase(pos,e->name.length());
			return e;
		}
	}
	return nullptr;
}



string extractPhrase(vector<string> phrasesToFind, string *userInput)
{

	strlensort(&phrasesToFind);
	string lcaseInput = " " + toLower(*userInput) + " ";
	for(auto phrase : phrasesToFind)
	{
		string lcasePhrase = " " + toLower(phrase) + " ";
		size_t pos = lcaseInput.find(lcasePhrase);
		if(pos != string::npos)
		{
			userInput->erase(pos,phrase.length());
			return phrase;
		}
	}
	return "";
}
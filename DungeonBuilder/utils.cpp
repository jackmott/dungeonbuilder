#include "string_constants.h"
#include "dungeon_room.h"
#include "dungeon_player.h"
#include "dungeon_object.h"
#include "dungeon_entity.h"
#include "utils.h"
#include <sstream>
#include <algorithm>
#include "lib.h"

using namespace std;

unsigned long getUID() { return global_id++; }

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
		string lowerWord = toLower(word);

		if(lowerWord != "a" && lowerWord != "the" && lowerWord != "my")
		{
			result.push_back(word);
		}
	}
	return result;
}



ObjectTarget extractObjectTarget(vector<string> words)
{
	ObjectTarget result;
	for(size_t i = 0; i < words.size(); i++)
	{
		string lowerWord = toLower(words[i]);

		if(lowerWord == "with" || lowerWord == "using") //target -> object
		{
			for(size_t j = 0; j < i; j++)
			{
				result.target += words[j];
			}
			for(size_t j = i+1; j < words.size(); j++)
			{
				result.object += words[j];
			}
			break;
		}
		else if(lowerWord == "on" || lowerWord == "at" || lowerWord == "on" || lowerWord == "toward")  //object -> target
		{
			for(size_t j = 0; j < i; j++)
			{
				result.object += words[j];
			}
			for(size_t j = i+1; j < words.size(); j++)
			{
				result.target += words[j];
			}
			break;
		}
	}


	return result;

}



vector<string> split(const string &s,char delim) {
	vector<string> elems;
	split(s,delim,elems);
	return elems;
}

vector<string> splitOnSpaceAndEnter(const string &s)
{
	vector<string> result;

	for(size_t i = 0; i < s.size();i++)
	{
		size_t spaceIndex = s.find(CHR_SPACE,i);
		size_t newlineIndex = s.find(CHR_NEWLINE,i);

		if(spaceIndex <= newlineIndex) {
			if(spaceIndex == string::npos) {
				result.push_back(s.substr(i,s.size()-i));
				break;
			}
			else {
				result.push_back(s.substr(i,spaceIndex-i));
				i = spaceIndex;
				string spaceChunk;
				while(i < s.size() && s[i] == CHR_SPACE)
				{
					spaceChunk += CHR_SPACE;
					i++;
				}
				i--;
				result.push_back(spaceChunk);
			}
		}
		else
		{
			result.push_back(s.substr(i,newlineIndex-i+1));
			i = newlineIndex;
		}
	}
	return result;
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

string join(unsigned int offset,vector<string> const &v,char delim)
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
		return first->getPrimaryName().size() > second->getPrimaryName().size();
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

	if(l == "t" || l=="y" || l=="yes" || l == "true") return true;
	return false;
}

string thereIsA(string thing)
{
	return "There is " + a_an(thing) + " here";
}



bool removeStr(vector<string> *strs,string str)
{
	int i = 0;
	str = toLower(str);

	for(auto s : *strs)
	{
		s = toLower(s);
		if(s == str)
		{
			strs->erase(strs->begin()+i);
			return true;
		}
		i++;
	}
	return false;
}

void removePointer(void* _pointers,void* pointer)
{
	vector<void*> *pointers = (vector<void*>*)_pointers;
	int i = 0;
	for(auto p : *pointers)
	{
		if(p == pointer)
		{
			pointers->erase(pointers->begin()+i);
			break;
		}
		i++;
	}
}

// void* considered harmful. living dangerously.
DungeonEntity* extractEntity(void * _entities,string *userInput,int* matchedName)
{
	vector<DungeonEntity*> *entities = (vector<DungeonEntity*> *)_entities;
	vector<DungeonEntity*> sortedEntities;

	for(auto e: *entities)
	{
		sortedEntities.push_back(e);
	}

	entitysort(&sortedEntities);
	string lcaseInput = toLower(*userInput);
	for(auto e : sortedEntities)
	{
		vector<string> lcaseNames = e->getLcaseNames();
		//strlensort(&lcaseNames);
		int i = 0;
		for(auto s : lcaseNames)
		{
			if(lcaseInput.compare(s) == 0)
			{
				if(matchedName != nullptr) *matchedName = i;
				return e;
			}
			i++;
		}
	}
	return nullptr;
}

DungeonObject* extractAndRemoveObject(vector<DungeonObject*> * objects,string *userInput)
{
	DungeonObject* result = (DungeonObject*)extractEntity(objects,userInput);
	if(result != nullptr) {
		removePointer(objects,result);
		return result;
	}
	for(auto o : *objects)
	{

		if(o->isOpen && o->contents.size() > 0)
		{
			result = extractAndRemoveObject(&o->contents,userInput);
			if(result != nullptr) {
				removePointer(&o->contents,result);
				return result;
			}

		}
	}
	return nullptr;
}


DungeonObject* extractObject(vector<DungeonObject*> * objects,string *userInput,int* matchedName)
{
	DungeonObject* result = (DungeonObject*)extractEntity(objects,userInput,matchedName);
	if(result != nullptr) {
		return result;
	}

	for(auto o : *objects)
	{

		if(o->isOpen && o->contents.size() > 0)
		{
			result = extractObject(&o->contents,userInput,matchedName);
			if(result != nullptr) return result;

		}
	}
	return nullptr;
}

void trim(string* str)
{
	str->erase(str->begin(),find_if(str->begin(),str->end(),
		[](char& ch)->bool { return !isspace(ch); }));
	str->erase(find_if(str->rbegin(),str->rend(),
		[](char& ch)->bool { return !isspace(ch); }).base(),str->end());

}


string extractPhrase(vector<string> phrasesToFind,string *userInput)
{

	strlensort(&phrasesToFind);
	string lcaseInput = CHR_SPACE + toLower(*userInput) + CHR_SPACE;
	for(auto phrase : phrasesToFind)
	{
		string lcasePhrase = CHR_SPACE + toLower(phrase) + CHR_SPACE;
		size_t pos = lcaseInput.find(lcasePhrase);
		if(pos != string::npos)
		{
			userInput->erase(pos,phrase.length());
			trim(userInput);
			return phrase;
		}
	}
	return "";
}

int min(int a,int b)
{
	if(a < b) return a;
	else return b;
}
int max(int a,int b)
{
	if(a > b) return a;
	else return b;
}

#ifdef _WIN32
void dbsleep(unsigned int milliseconds)
{
	//Sleep(milliseconds);
}
#else
void dbsleep(unsigned int milliseconds)
{
	usleep(milliseconds * 1000); // takes microseconds
}
#endif


void recursiveObjectAccumulator(vector<DungeonObject*> *result,vector<DungeonObject*> *objects)
{
	(*result).insert(result->end(),objects->begin(),objects->end());
	for(auto o : *objects)
	{
		if(o->contents.size() > 0)
		{
			recursiveObjectAccumulator(result,&o->contents);
		}
	}
}

vector<DungeonObject*> getAllPlayerObjects(DungeonPlayer* player)
{
	vector<DungeonObject*> result;
	recursiveObjectAccumulator(&result,&player->objects);
	return result;

}

vector<DungeonObject*> getAllRoomObjects(DungeonRoom* room)
{
	vector<DungeonObject*> result;
	recursiveObjectAccumulator(&result,&room->objects);
	return result;
}

vector<DungeonObject*> getAllObjects(DungeonPlayer* player,DungeonRoom* room)
{
	vector<DungeonObject*> result = getAllPlayerObjects(player);
	vector<DungeonObject*> resultB = getAllRoomObjects(room);

	result.insert(result.end(),resultB.begin(),resultB.end());
	return result;
}

DungeonEntity* getEntityById(void* v,int uid)
{
	vector<DungeonEntity*> *eee = (vector<DungeonEntity*>*)v;
	for(auto e : *eee)
	{
		if (e->uid == uid) return e;
	}
	return nullptr;
}
#include "dungeon_serializer.h"
#include "string_constants.h"
#include "dungeon_room.h"
#include "dungeon_object.h"
#include "dungeon_exit.h"
#include "dungeon_creature.h"

using namespace std;


string _writeInt(string name,int value)
{
	return STR_QUOT + name + STR_QUOT + ":" + to_string(value) +",";
}

string _writeString(string name,string value)
{
	return STR_QUOT + name +STR_QUOT+ ":" + STR_QUOT + value + STR_QUOT+",";
}

string _writeBool(string name,bool value)
{
	return STR_QUOT + name +STR_QUOT + ":" + to_string(value)+",";
}

string _writeVectorString(string name,vector<string> const &value)
{
	string result =  STR_QUOT + name + STR_QUOT+ ":[";
	for(unsigned int i = 0; i < value.size(); i++)
	{
		result += STR_QUOT + value[i] + STR_QUOT;
		if(i != value.size()-1) {
			result += ",";
		}
	}
	result = result +"],";
	return result;
}

//This only writes out the ids
string _writeVectorEntity(string name,void *value)
{
	vector<DungeonEntity*> *entityValue = (vector<DungeonEntity*>*)value;
	string result =  STR_QUOT + name + STR_QUOT+ ":[";
	for(auto v : *entityValue) {
		result += to_string(v->uid)+ ",";
	};
	result = result +"],";
	return result;
}


JSONLoader::JSONLoader(string filename)
{
	
	filename.append(".json");
	this->filename = filename;
	fin.open(filename.c_str());
	if (fin) {
		open = true;
		fin >> ch;
	}
	else open = false;
	donePass = false;
}
JSONLoader::~JSONLoader()
{
	fin.close();
}

vector<DungeonRoom*> JSONLoader::loadMap()
{
	vector<DungeonRoom*> roomList;
	if (getJSONEntry( ))
	{
		if (currEntry[0] == "rooms")
			while (getJSONEntry( ))
				roomList.push_back(loadRoom(roomList));
	} 
	donePass = true;
	fin.seekg(0, ios::beg);
	getExits(roomList);

	return roomList;
}

DungeonRoom* JSONLoader::loadRoom( vector<DungeonRoom*> roomList)
{
	DungeonRoom* room = new DungeonRoom;
	do
	{
		if (currEntry[0] == "uid")
			room->uid = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "name")
			while( ch != ']' &&  getJSONEntry( ))
				room->addName(currEntry[1]);
		else if (currEntry[0] == "description")
			while (ch != ']' &&  getJSONEntry( ))
				room->description.push_back(currEntry[1]);
		else if (currEntry[0] == "objects")
			while( getJSONEntry( ) )
				room->objects.push_back(loadObject());
		else if (currEntry[0] == "creatures")
			while (getJSONEntry( ))
				room->creatures.push_back(loadCreature());
		else if (currEntry[0]=="exits")
			while (getJSONEntry( ))
				loadExit(roomList);
	} while (ch != '}' && getJSONEntry( ));

	return room;
}

DungeonObject* JSONLoader::loadObject()
{
	DungeonObject* object = new DungeonObject;

	do
	{
		if (currEntry[0] == "name")
			while (ch != ']' &&  getJSONEntry( ))
				object->addName(currEntry[1]);
		else if (currEntry[0] == "description")
			while (ch != ']' &&  getJSONEntry( ))
				object->description.push_back(currEntry[1]);		
		else if (currEntry[0] == "mass")
			object->mass = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "size")
			object->size = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "canOpen")
			object->canOpen = currEntry[1].c_str() == STR_JSON_TRUE;
		else if (currEntry[0] == "canTake")
			object->canTake = currEntry[1].c_str() == STR_JSON_TRUE;
		else if (currEntry[0] == "isOpen")
			object->isOpen = currEntry[1].c_str() == STR_JSON_TRUE;
		/* else if (currEntry[0] == "useAliases")
			while (ch != ']' &&  getJSONEntry())
				object->useAliases.push_back(currEntry[1]);*/  //todo - usealias replaced by actions
		else if (currEntry[0] == "contents")
			while (getJSONEntry())
				object->contents.push_back(loadObject());
	} while (ch != '}' && getJSONEntry( ));
	
	return object;
}

DungeonCreature* JSONLoader::loadCreature()
{
	DungeonCreature* creature = new DungeonCreature;

	do
	{

		if (currEntry[0] == "name")
			while (ch != ']' &&  getJSONEntry( ))
				creature->addName(currEntry[1]);
		else if (currEntry[0] == "description")
			while (ch != ']' &&  getJSONEntry( ))
				creature->description.push_back(currEntry[1]);
		else if (currEntry[0] == "hitpoints")
			creature->hitpoints = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "alignment")
			creature->alignment = atoi(currEntry[1].c_str());
	} while (ch != '}' && getJSONEntry( ));

	return creature;
}

int _loadInt(string name,string json)
{
	//todo get the int at name from the json
	return 5;
}

void JSONLoader::getExits(vector<DungeonRoom*> roomList)
{
	for (DungeonRoom* room : roomList)
	{	
		getJSONEntry( );
		while (currEntry[0] != "exits")
			getJSONEntry( );
		while (getJSONEntry( ))
		{
			room->exits.push_back(loadExit(roomList));
		}
		getJSONEntry( );
	}
}

DungeonExit* JSONLoader::loadExit( vector<DungeonRoom*> roomList)
{
	DungeonExit* exit = new DungeonExit;

	do
	{
		if (donePass)
		{			
			if (currEntry[0] == "name")
				while (ch != ']' &&  getJSONEntry( ))
					exit->addName(currEntry[1]);			
			else if (currEntry[0] == "links")
				exit->room = roomList[atoi(currEntry[1].c_str())];
			else if (currEntry[0] == "isDoor")
				exit->isDoor = atoi(currEntry[1].c_str());
			else if (currEntry[0] == "isOpen")
				exit->isOpen = atoi(currEntry[1].c_str());
			else if (currEntry[0] == "distance")
				exit->distance = atoi(currEntry[1].c_str());
			else if (currEntry[0] == "openingText")
				exit->openingText = currEntry[1];
			else if (currEntry[0] == "closingText")
				exit->closingText = currEntry[1];
			else if (currEntry[0] == "openText")
				exit->openText = currEntry[1];
			else if (currEntry[0] == "closedText")
				exit->closedText = currEntry[1];
		}
	} while (ch != '}' && getJSONEntry( ));

	return exit;
}
/*
[split] -- Splits entry string on ':' and removes quotes from entry
@param string entryString - a string of a full JSON entity 
*/
void JSONLoader::split(string entryString)
{
	unsigned int i = 0;
	while (entryString[i] != ':')
	{
		currEntry[0].push_back(entryString[i]);
		++i;
	}
	i += 1;
	while ( i < entryString.length() )
	{
		currEntry[1].push_back(entryString[i]);
		++i;
	}
}

 /*
 [getJSONLoader]
 @param fin    ifstream& - input file object which is used for whole json file reading
 @param ch     char&     - charachter which stores current char in file
 @return  bool  returns true if there was an entry before end of current brackets
 */
bool JSONLoader::getJSONEntry()
{	
	string curr;
	if (!ch || ch == '}' || ch == ']')
		fin >> ch;
	while (ch != '\"' && !isdigit(ch) && (ch != '}' && ch != ']'))
		fin >> ch;
	if (ch != '\"')
		curr.push_back(ch);

	bool inQuote = ch == '\"' ? true : false;
	char prev = ch;
	bool single = true;
	if ( ch != '}' && ch != ']')
	{
		while (inQuote || (ch != ',' && ch != '}' && ch != '[' && ch != ']'))
		{
			fin >> noskipws >> ch;
			if (ch == '\"' && prev != '\\')
				inQuote = inQuote ? false : true;
			else if (inQuote || (ch != ',' && ch != ' ' && ch != '}' && ch != ']') )
			{
				curr.push_back(ch);
				if (ch == ':')
					single = false;
			}
			prev = ch;
		}
		currEntry[0] = currEntry[1] = "";
		if (!single)
			split(curr);
		else
			currEntry[1] = curr;
		return true;
	}

	else return false;
}
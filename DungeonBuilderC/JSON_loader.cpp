#include "JSON_loader.h"
#include "string_constants.h"


using namespace std;
JSONLoader::JSONLoader(string filename)
{
	char ch = ' ';
	filename.append(".json");
	this->filename = filename;
	fin.open(filename.c_str());
	if (fin)
		open = true;
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
	if (getJSONEntry())
	{
		if (currEntry[0] == "rooms")
			while (getJSONEntry())
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
			while( getJSONEntry() )
				room->addName(currEntry[1]);
		else if (currEntry[0] == "description")
			while( getJSONEntry(true) )
				room->description.push_back(currEntry[1]);
		else if (currEntry[0] == "objects")
			while( getJSONEntry() )
				room->objects.push_back(loadObject());
		else if (currEntry[0] == "creatures")
			while (getJSONEntry())
				room->creatures.push_back(loadCreature());
		else if (currEntry[0]=="exits")
				while (getJSONEntry())
					loadExit(roomList);
	} while (ch != '}' && getJSONEntry());

	return room;
}

DungeonObject* JSONLoader::loadObject()
{
	DungeonObject* object = new DungeonObject;

	do
	{
		if (currEntry[0] == "name")
			while( getJSONEntry() )
				object->addName(currEntry[1]);
		else if (currEntry[0] == "description")
			while( getJSONEntry(true) )
				object->description.push_back(currEntry[1]);
		else if (currEntry[0] == "damage")
			object->damage = atoi(currEntry[1].c_str());
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
	} while (ch != '}' && getJSONEntry());
	
	return object;
}

DungeonCreature* JSONLoader::loadCreature()
{
	DungeonCreature* creature = new DungeonCreature;

	do
	{

		if (currEntry[0] == "name")
			while( getJSONEntry() )
				creature->addName(currEntry[1]);
		else if (currEntry[0] == "description")
			while( getJSONEntry(true) )
				creature->description.push_back(currEntry[1]);
		else if (currEntry[0] == "hitpoints")
			creature->hitpoints = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "alignment")
			creature->alignment = atoi(currEntry[1].c_str());
	} while (ch != '}' && getJSONEntry());

	return creature;
}

void JSONLoader::getExits(vector<DungeonRoom*> roomList)
{
	for (DungeonRoom* room : roomList)
	{	
		getJSONEntry();
		while (currEntry[0] != "exits")
			getJSONEntry();
		while (getJSONEntry())
		{
			room->exits.push_back(loadExit(roomList));
		}
		getJSONEntry();
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
				while( getJSONEntry() )
					exit->addName(currEntry[1]);			
			else if (currEntry[0] == "links")
				exit->room = roomList[atoi(currEntry[1].c_str())];
		}
	} while (ch != '}' && getJSONEntry());

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
		if(entryString[i] != '\"')
			currEntry[0].push_back(entryString[i]);
		++i;
	}
	i += 1;
	if (entryString[i] == '\"')
	{
		++i;
		while (entryString[i] != '\"')
		{
			currEntry[1].push_back(entryString[i]);
			++i;
		}
	}
	else
	{
		while ( i < entryString.length() )
		{
			currEntry[1].push_back(entryString[i]);
			++i;
		}
	}
}

 /*
 [getJSONLoader]
 @param fin    ifstream& - input file object which is used for whole json file reading
 @param ch     char&     - charachter which stores current char in file
 @return  bool  returns true if there was an entry before end of current brackets
 */
bool JSONLoader::getJSONEntry(bool single)
{	
	string curr;
	if (ch == '}' || ch == ']')
		fin >> ch;
	if (!single)
		while (ch != '\"' && (ch != '}' && ch != ']'))
			fin >> ch;
	else
		while ((ch != '\"' || !isdigit(ch)) && (ch != '}' && ch != ']'))
			fin >> ch;
	curr.push_back(ch);

	char prev = ch;
	if (!single && ch != '}' && ch != ']')
	{
		bool inQuote = true;
		while (inQuote || (ch != ',' && ch != '}' && ch != '['))
		{
			fin >> noskipws >> ch;
			if (ch == '\"' && prev != '\\')
				inQuote = inQuote ? false : true;
			if (inQuote || ch != ' ' && ch!= '}')
				curr.push_back(ch);

			prev = ch;
		}
		currEntry[0] = currEntry[1] = "";
		split(curr);
		return true;
	}
	else if (single && ch != '}' && ch != ']')
	{
		while (ch != ',' && ch != '}' && ch != '[')
		{
			fin >> noskipws >> ch;
			if (ch != '}')
				curr.push_back(ch);
		}
		currEntry[0] = currEntry[1] = "";
		currEntry[1] = curr;
		return true;
	}

	else return false;
}
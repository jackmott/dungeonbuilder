#include "headers/JSON_loader.h"

using namespace std;

vector<DungeonRoom*> JSONLoader::loadMap(ifstream& fin)
{

	string word;
	char ch;
	stack<char> symStack;
	vector<DungeonRoom*> roomList;

	while (word != "\"Rooms\":")
		fin >> word;
	fin >> ch;
	symStack.push('}');
	symStack.push(']');
	int numRooms = 0;
	string curr;
	while (!symStack.empty()) 
	{
		fin >> ch;
		curr.push_back(ch);
		if (ch == '{')
			roomList.push_back(loadRoom(fin, ch, roomList));
		else
			symStack.pop();
	}

	return roomList;
}

DungeonRoom* JSONLoader::loadRoom(ifstream& fin, char & ch, vector<DungeonRoom*> roomList)
{
	DungeonRoom* room = new DungeonRoom;
	while( ch != ',' )
	{
		string entry[2];
		getJSONEntry(fin, ch, entry);
		if (entry[0] == "ID")
			room->uid = atoi(entry[1].c_str());
		else if (entry[0] == "Name")
			room->name = entry[1];
		else if (entry[0] == "Description")
			room->description = entry[1];
		else if (entry[0] == "Objects")
			room->objects.push_back(loadObject(fin, ch));
		else if (entry[0] == "Creatures")
			room->creatures.push_back(loadCreature(fin, ch));
		else
			break;
	}

	return room;
}

DungeonObject* JSONLoader::loadObject(ifstream & fin, char & ch)
{
	DungeonObject* object = new DungeonObject;

	while( ch != ',')
	{
		string entry[2];
		getJSONEntry(fin, ch, entry);
		if (entry[0] == "Name")
			object->name = entry[1];
		else if (entry[0] == "Description")
			object->description = entry[1];

		fin >> noskipws >> ch;
	}
	
	return object;
}

DungeonCreature* JSONLoader::loadCreature(ifstream & fin, char & ch)
{
	DungeonCreature* creature = new DungeonCreature;

	while (ch != ',')
	{
		string entry[2];
		getJSONEntry(fin, ch, entry);

		if (entry[0] == "Name")
			creature->name = entry[1];
		else if (entry[0] == "Description")
			creature->description == entry[1];
		else if (entry[0] == "hitpoints")
			creature->hitpoints == atoi(entry[1].c_str());
		else if (entry[0] == "Alignment")
			creature->hitpoints == atoi(entry[1].c_str());

		fin >> noskipws >> ch;
	}

	return creature;
}

DungeonExit* JSONLoader::loadExit(ifstream & fin, char & ch, vector<DungeonRoom*> roomList)
{
	string curr;
	DungeonExit* exit = new DungeonExit;

	while (fin >> curr && curr != "],")
	{
		string entry[2];
		split(curr, entry);
		if (entry[0] == "Name")
			exit->name = entry[1];
		else if (entry[0] == "Description")
			exit->description == entry[1];
		else if (entry[0] == "links")
			exit->room == roomList[atoi(entry[1].c_str())];
	}

	return exit;
}

void JSONLoader::split(string entry, string parts[2])
{
	int i = 0;
	while (entry[i] != ':')
	{
		if(entry[i] != '\"')
			parts[0].push_back(entry[i]);
		++i;
	}
	i += 1;
	if (entry[i] == '\"')
	{
		++i;
		while (entry[i] != '\"')
		{
			parts[1].push_back(entry[i]);
			++i;
		}
	}
	else
	{
		while ( i < entry.length() )
		{
			parts[1].push_back(entry[i]);
			++i;
		}
	}
}

void JSONLoader::getJSONEntry(ifstream& fin, char& ch, string entry[2]) 
{	
	string curr;
	while (ch != '\"')
		fin >> ch;
	curr.push_back(ch);

	bool inQuote = true;

	char prev = ch;
	while (fin >> noskipws >> ch && (inQuote || (ch != ',' && ch != '}' && ch != '[')))
	{
		if (ch == '\"' && prev != '\\')
			inQuote = inQuote ? false : true;
		if (inQuote || ch != ' ')
			curr.push_back(ch);

		
		prev = ch;
	}
	
	split(curr, entry);
}
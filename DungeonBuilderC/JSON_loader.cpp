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
	while (!symStack.empty()) 
	{
		fin >> ch;
		if (ch == '{')
			roomList.push_back(this->loadRoom(fin, roomList));
		else
			symStack.pop();
	}

	return roomList;
}

DungeonRoom* JSONLoader::loadRoom(ifstream& fin, vector<DungeonRoom*> roomList)
{
	string curr;
	DungeonRoom* room = new DungeonRoom;
	while (fin >> curr && curr != "\"Objects\":[" )
	{
		string entry[2];
		split(curr, entry);
		if (entry[0] == "ID")
			room->uid = atoi(entry[1].c_str());
		else if (entry[0] == "ID")
			room->name = entry[1];
		else if (entry[0] == "Description")
			room->description = entry[1];
	}
	while (fin >> curr && curr != "],")
		room->objects.push_back( loadObjects(fin) );
	while (fin >> curr && curr != "],")
		room->creatures.push_back( loadCreatures(fin) );
	while (fin >> curr && curr != "]")
		room->exits.push_back( loadExits(fin,  roomList) );

	return room;
}

DungeonObject* JSONLoader::loadObjects(ifstream & fin)
{
	string curr;
	DungeonObject* object = new DungeonObject;

	while (fin >> curr && curr != "],")
	{
		string entry[2];
		split(curr, entry);
		if (entry[0] == "Name")
			object->name = entry[1];
		else if (entry[0] == "Description")
			object->description == entry[1];
	}

	return object;
}

DungeonCreature* JSONLoader::loadCreatures(ifstream & fin)
{
	string curr;
	DungeonCreature* creature = new DungeonCreature;

	while (fin >> curr && curr != "],")
	{
		string entry[2];
		split(curr, entry);
		if (entry[0] == "Name")
			creature->name = entry[1];
		else if (entry[0] == "Description")
			creature->description == entry[1];
		else if (entry[0] == "hitpoints")
			creature->hitpoints == atoi(entry[1].c_str());
		else if (entry[0] == "Alignment")
			creature->hitpoints == atoi(entry[1].c_str());
	}

	return creature;
}

DungeonExit* JSONLoader::loadExits(ifstream & fin, vector<DungeonRoom*> roomList)
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
	i += 2;
	if (entry[i] == '\"')
	{
		while (entry[i] != '/"')
		{
			parts[1].push_back(entry[i]);
			++i;
		}
	}
	else
	{
		while (entry[i] != ',' && entry[i] != '}')
		{
			parts[1].push_back(entry[i]);
			++i;
		}
	}
}
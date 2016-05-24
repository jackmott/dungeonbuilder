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
			roomList.push_back(this->loadRoom(fin));
		else
			symStack.pop();
	}

	return roomList;
}

DungeonRoom* JSONLoader::loadRoom(ifstream& fin)
{
	char ch;
	string curr;
	DungeonRoom* room = new DungeonRoom;
	while (fin >> curr && curr != "\"Objects\":[" )
	{
		string entry[2];
		split(curr, entry);
		if (entry[0] == "ID")
			room->uid = static_cast<int>(entry[1]);
		else if (entry[0] == "ID")
			room->name = entry[1];
		else if (entry[0] == "Description")
			room->description = entry[1];
	}
	return room;
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
	++i;
	while (entry[i] != ',')
	{
		if (entry[i] != '\"')
			parts[1].push_back(entry[i]);
		++i;
	}
}
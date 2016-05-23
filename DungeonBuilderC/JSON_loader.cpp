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
	while(fin >> ch)
	return room;
}
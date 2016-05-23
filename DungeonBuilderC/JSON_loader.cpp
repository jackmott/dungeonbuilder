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
	while (!symStack.empty()) {
		roomList.push_back(this->loadRoom(fin));
	}

	return roomList;
}

DungeonRoom* JSONLoader::loadRoom(ifstream& fin)
{
	DungeonRoom* room = new DungeonRoom;

	return room;
}
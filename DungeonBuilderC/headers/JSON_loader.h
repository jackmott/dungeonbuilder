#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include <string>
#include <fstream>
#include <stack>
#include <vector>


struct DungeonRoom;
struct DungeonObject;
struct DungeonCreature;
struct DungeonExit;

struct JSONLoader {
	char ch;
	std::ifstream fin;
	std::string filename;
	std::string currEntry[2];
	bool donePass;
	bool open;
	bool single;
	
	JSONLoader(std::string);
	~JSONLoader();
	std::vector<DungeonRoom*> loadMap();
	DungeonRoom* loadRoom(std::vector<DungeonRoom*> roomList);
	DungeonObject* loadObject();
	DungeonCreature* loadCreature();

	void getExits(std::vector<DungeonRoom*> roomList);
	DungeonExit* loadExit( std::vector<DungeonRoom*> roomList);
	bool getJSONEntry();
	
	void split(std::string);
};

#endif
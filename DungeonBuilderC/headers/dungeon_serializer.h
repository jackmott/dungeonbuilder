#ifndef DUNGEON_SERIALIZER_H
#define DUNGEON_SERIALIZER_H

#include <string>
#include <fstream>
#include <stack>
#include <vector>

struct DungeonEntity;

//reflection, the C way
// #x expands to a string literal of the variable name
#define writeInt(x) _writeInt(#x,x)
#define writeString(x) _writeString(#x,x)
#define writeBool(x) _writeBool(#x,x)
#define writeVectorString(x) _writeVectorString(#x,x)

//only the ids!
#define writeVectorEntity(x) _writeVectorEntity(#x,&x);

struct DungeonRoom;
struct DungeonObject;
struct DungeonCreature;
struct DungeonExit;


std::string _writeInt(std::string name, int value);
std::string _writeString(std::string name,std::string value);
std::string _writeBool(std::string name,bool value);
std::string _writeVectorString(std::string name,std::vector<std::string> const &value);

//only the ids!
std::string _writeVectorEntity(std::string name, void *value);


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
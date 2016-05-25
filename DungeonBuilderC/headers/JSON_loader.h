#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include "dungeon_data.h"

using namespace std;

struct JSONLoader {
	char ch;
	ifstream fin;
	string currEntry[2];
	bool open;
	
	JSONLoader(string);
	~JSONLoader();
	vector<DungeonRoom*> loadMap();
	DungeonRoom* loadRoom(vector<DungeonRoom*> roomList);
	DungeonObject* loadObject();
	DungeonCreature* loadCreature();
	DungeonExit* loadExit( vector<DungeonRoom*> roomList);
	bool getJSONEntry();
	
	void split(string);
};

#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include "dungeon_data.h"

using namespace std;

struct JSONLoader {
	vector<DungeonRoom*> loadMap(ifstream&);
	DungeonRoom* loadRoom(ifstream&, char& ,vector<DungeonRoom*> roomList);
	DungeonObject* loadObject(ifstream&, char&);
	DungeonCreature* loadCreature(ifstream&, char&);
	DungeonExit* loadExit(ifstream&, char&, vector<DungeonRoom*> roomList);
	void getJSONEntry(ifstream&, char&, string[2]);
	
	void split(string, string[2]);
};

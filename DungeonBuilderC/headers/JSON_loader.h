#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include "dungeon_data.h"

using namespace std;

struct JSONLoader {
	vector<DungeonRoom*> loadMap(ifstream&);
	DungeonRoom* loadRoom(ifstream&, vector<DungeonRoom*> roomList);
	DungeonObject* loadObjects(ifstream& fin);
	DungeonCreature* loadCreatures(ifstream& fin);
	DungeonExit* loadExits(ifstream& fin, vector<DungeonRoom*> roomList);
	void split(string, string[2]);
};

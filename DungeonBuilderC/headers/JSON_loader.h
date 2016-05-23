#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include "dungeon_data.h"

using namespace std;

struct JSONLoader {
	vector<DungeonRoom*> loadMap(ifstream&);
	DungeonRoom* loadRoom(ifstream&);
};

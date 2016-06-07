#include "dungeon_action.h"
#include "utils.h"
#include <sstream>

using namespace std;
extern vector<DungeonAction*> g_actionList;

DungeonAction::DungeonAction()
{
	uid = getUID();
	needToHold = true;
	g_actionList.push_back(this);
}

DungeonAction::~DungeonAction()
{

}

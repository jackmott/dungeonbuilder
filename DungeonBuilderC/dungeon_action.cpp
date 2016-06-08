#include "dungeon_action.h"
#include "dungeon_serializer.h"
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

DungeonAction::DungeonAction(string json)
{
	//todo make thyself
}

DungeonAction::~DungeonAction()
{

}

string DungeonAction::toJSON()
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeBool(needToHold);
	sout << writeVectorEntity(effects);

	return sout.str();
}

#include "dungeon_action.h"
#include "dungeon_serializer.h"
#include "utils.h"
#include "json.h"
#include <sstream>

using namespace std;
extern vector<DungeonAction*> g_actionList;

DungeonAction::DungeonAction()
{
	entityType = ENTITY_TYPE::Action;
	uid = getUID();
	needToHold = true;
	g_actionList.push_back(this);
}

DungeonAction::DungeonAction(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Action;
	loadInt(uid,json);
	loadBool(needToHold,json);
	loadVectorString(names,json);
}

DungeonAction::~DungeonAction()
{

}

string DungeonAction::toJSON()
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeVectorString(names);
	sout << writeBool(needToHold);
	sout << writeVectorEntity(effects);
	sout << writeVectorEntity(targets);

	return sout.str();
}

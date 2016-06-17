#include "dungeon_action.h"
#include "dungeon_serializer.h"
#include "utils.h"
#include "json.h"
#include <sstream>

using namespace std;
extern GlobalState globalState;

DungeonAction::DungeonAction()
{
	entityType = ENTITY_TYPE::Action;
	uid = getUID();
	needToHold = true;
	globalState.actionList.push_back(this);
	globalState.entityList.push_back(this);
}

DungeonAction::DungeonAction(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Action;
	loadInt(uid,json);
	loadEntity(parent,json);
	loadBool(needToHold,json);
	loadString(output,json);
	loadVectorString(names,json);
	loadVectorEntity(targets,json);
	loadVectorEntity(effects,json);
	globalState.actionList.push_back(this);
	globalState.entityList.push_back(this);
}

void DungeonAction::fixUpPointers()
{
	for(int i = 0; i < targets.size();i++)
	{
		targets[i] = (DungeonEntity*)getEntityById(&globalState.entityList,(int)targets[i]);
	}
	for(int i = 0; i < effects.size();i++)
	{
		effects[i] = (DungeonEffect*)getEntityById(&globalState.entityList,(int)effects[i]);
	}
	if (parent != (DungeonEntity*)-1)
		parent = (DungeonEntity*)getEntityById(&globalState.entityList,(int)parent);
	else
		parent = nullptr;
}

DungeonAction::~DungeonAction()
{

}

string DungeonAction::toJSON()
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeVectorString(names);
	sout << writeBool(needToHold);
	sout << writeString(output);
	sout << writeVectorEntity(effects);
	sout << writeVectorEntity(targets);

	return sout.str();
}

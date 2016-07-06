#include "dungeon_action.h"
#include "dungeon_room.h"
#include "dungeon_object.h"
#include "dungeon_serializer.h"
#include "utils.h"
#include "json.h"
#include "dungeon_effect.h"
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
	onetime = false;
}

DungeonAction::DungeonAction(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Action;
	loadInt(uid,json);
	loadEntity(parent,json);
	loadBool(needToHold,json);
	loadBool(onetime,json);
	loadString(output,json);
	loadVectorString(names,json);
	loadVectorEntity(targets,json);
	loadVectorEntity(effects,json);
	globalState.actionList.push_back(this);
	globalState.entityList.push_back(this);
}

string DungeonAction::apply(vector<string>*textBuffer, DungeonEntity* targetEntity, DungeonPlayer* player, DungeonRoom* room,bool objectOnPlayer)
{
	for(auto e : effects)
	{
		e->apply(textBuffer,targetEntity,player,room,objectOnPlayer);
	}
	string result = output;
	//if we are a one time use action, remove ourself from the universe	
	if (onetime)
	{
		if (parent->entityType == ENTITY_TYPE::Room)
		{
			DungeonRoom* room = (DungeonRoom*)parent;
			removePointer(&room->actions,this);
		} else if (parent->entityType == ENTITY_TYPE::Object)
		{
			DungeonObject* object = (DungeonObject*)parent;
			removePointer(&object->actions,this);
		}
		delete this;
	}
	return result;
}

void DungeonAction::fixUpPointers()
{
	for(size_t i = 0; i < targets.size();i++)
	{
		targets[i] = (DungeonEntity*)getEntityById(&globalState.entityList,(int)targets[i]);
	}
	for(size_t i = 0; i < effects.size();i++)
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
	removePointer(&globalState.actionList,this);
	removePointer(&globalState.entityList,this);
}

string DungeonAction::toJSON()
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeVectorString(names);
	sout << writeBool(needToHold);
	sout << writeBool(onetime);
	sout << writeString(output);
	sout << writeVectorEntity(effects);
	sout << writeVectorEntity(targets);

	return sout.str();
}

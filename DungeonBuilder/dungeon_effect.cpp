#include "dungeon_creature.h"
#include "dungeon_effect.h"
#include "dungeon_room.h"
#include "dungeon_player.h"
#include "dungeon_object.h"
#include "dungeon_action.h"
#include "dungeon_serializer.h"
#include <sstream>
#include "utils.h"
#include "json.h"

using namespace std;

extern vector<DungeonEffect*> g_effectList;
extern vector<DungeonEntity*> g_entityList;
extern vector<DungeonObject*> g_objectList;

DungeonEffect::DungeonEffect()
{
	entityType = ENTITY_TYPE::Effect;
	magnitude = 0;
	type = EFFECT_TYPE::Heal;
	uid = getUID();
	g_effectList.push_back(this);
	g_entityList.push_back(this);
}

DungeonEffect::DungeonEffect(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Effect;
	loadInt(uid,json);	
	loadEntity(parent,json);
	type = (EFFECT_TYPE)loadEnum(type,json);
	loadString(output,json);
	loadInt(speed,json);
	loadVectorEntity(transforms,json);
	g_effectList.push_back(this);
	g_entityList.push_back(this);
	
}
DungeonEffect::~DungeonEffect()
{
}

void DungeonEffect::fixUpPointers()
{
	for(int i = 0; i < transforms.size();i++)
	{
		transforms[i] = (DungeonObject*)getEntityById(&g_objectList,(int)transforms[i]);
	}
	if (parent != nullptr)
		parent = (DungeonEntity*)getEntityById(&g_entityList,(int)parent);
}
string DungeonEffect::getPrimaryName() const
{
	return EFFECT_STRS[(int)type];
}

void DungeonEffect::apply(vector<string> *textBuffer,DungeonEntity* target,DungeonPlayer* player,DungeonRoom * room,bool objectOnPlayer)
{
	switch(type)
	{
	case EFFECT_TYPE::Heal:
	{player->heal(magnitude);}
	break;
	case EFFECT_TYPE::Damage:
		//todo damage
		break;
	case EFFECT_TYPE::Transform:
	{DungeonObject *toTransform = (DungeonObject*)parent->parent;
	if(objectOnPlayer) {
		removePointer(&player->objects,toTransform);
		for(auto e : transforms)
		{
			player->objects.push_back(e);
		}
	}
	else {
		removePointer(&room->objects,toTransform);
		for(auto e : transforms) {
			room->objects.push_back(e);
		}
	}}
	break;
	case EFFECT_TYPE::Attack:
	{}
	break;
	}

}

//TODO: add enum classes for type
string DungeonEffect::toJSON()
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeEnum(type);
	sout << writeString(output);
	sout << writeInt(magnitude);
	sout << writeInt(speed);
	sout << writeVectorEntity(transforms);

	return sout.str();
}
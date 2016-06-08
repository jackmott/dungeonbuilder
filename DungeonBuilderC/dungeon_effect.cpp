#include "dungeon_creature.h"
#include "dungeon_effect.h"
#include "dungeon_room.h"
#include "dungeon_player.h"
#include "dungeon_object.h"
#include "dungeon_action.h"
#include "dungeon_serializer.h"
#include <sstream>
#include "utils.h"

using namespace std;

extern vector<DungeonEffect*> g_effectList;


DungeonEffect::DungeonEffect()
{
	magnitude = 0;
	type = EFFECT_TYPE::Heal;
	uid = getUID();
	g_effectList.push_back(this);
}

DungeonEffect::DungeonEffect(string json)
{

}
DungeonEffect::~DungeonEffect()
{
}

string DungeonEffect::getPrimaryName() const
{
	return EFFECT_STRS[(int)type];
}

void DungeonEffect::apply(vector<string> *textBuffer,DungeonPlayer* player,DungeonRoom * room,bool objectOnPlayer)
{
	switch(type)
	{
	case EFFECT_TYPE::Heal:
		player->heal(magnitude);
		break;
	case EFFECT_TYPE::Damage:
		if(creatureTarget != nullptr)
		{
			creatureTarget->applyDamage(textBuffer,magnitude);
		}
		else if(objectTarget != nullptr)
		{
			objectTarget->applyDamage(textBuffer,magnitude);
		}
		
		break;
	case EFFECT_TYPE::Transform:
		DungeonObject *toTransform = (DungeonObject*)parent->parent;
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
		}

		break;
	}

}

//TODO: add enum classes for type
string DungeonEffect::toJSON() 
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeString(output);
	sout << writeInt(magnitude);
	sout << writeInt(speed);
	sout << _writeInt("creatureTarget",creatureTarget->uid);
	sout << _writeInt("objectTarget",objectTarget->uid);
	sout << writeVectorEntity(transforms);

	return sout.str();
}
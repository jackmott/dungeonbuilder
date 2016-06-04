
#include "dungeon_effect.h"
#include "dungeon_room.h"
#include "dungeon_player.h"
#include "dungeon_object.h"
#include "dungeon_action.h"
#include <sstream>
#include "utils.h"

using namespace std;


DungeonEffect::DungeonEffect()
{
	magnitude = 0;
	type = EFFECT_TYPE::HEAL;
	uid = getUID();
}
DungeonEffect::~DungeonEffect()
{
}

string DungeonEffect::getName()
{
	return EFFECT_STRS[(int)type];
}

void DungeonEffect::apply(DungeonPlayer* player,DungeonRoom * room,bool objectOnPlayer)
{
	switch(type)
	{
	case EFFECT_TYPE::HEAL:
		player->heal(magnitude);
		break;
	case EFFECT_TYPE::DAMAGE:
		break;
	case EFFECT_TYPE::TRANSFORM:
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
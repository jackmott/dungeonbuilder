#ifndef DUNGEON_TRIGGER_H
#define DUNGEON_TRIGGER_H

#include "utils.h"
#include "dungeon_entity.h"
#include <vector>
#include <string>

struct DungeonAction;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonObject;
struct DungeonCreature;

struct DungeonEffect;

#define TRIGGER_ENUM_LOOP(TRIGGER) \
	TRIGGER(Proximity)	  \
	TRIGGER(Evil)	  \
	TRIGGER(Age)
	


const std::string  TRIGGER_STRS[] = { TRIGGER_ENUM_LOOP(GEN_STRING) };
enum class TRIGGER_TYPE { TRIGGER_ENUM_LOOP(GEN_ENUM) };

struct DungeonTrigger : DungeonEntity
{
	DungeonTrigger();
	DungeonTrigger(void* _json);
	~DungeonTrigger();

			
	TRIGGER_TYPE type;
	
	bool needToHold;
	std::string output;

	int magnitude;
	std::vector<DungeonObject*> objects;
	std::vector<DungeonRoom*> rooms;
	std::vector<DungeonCreature*> creatures;

	std::vector<DungeonEffect*> effects;
	

	virtual std::string  getPrimaryName() const override;
	
	bool checkForEvil(DungeonRoom* room,int depth);
	bool checkAge();
	
	std::string toJSON();
};

#endif

#pragma once
#include <string>
#include <vector>

using namespace std;


struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;

struct DungeonRoom
{
	string name;
	string description;
	vector<DungeonExit*> exits;
	vector<DungeonObject*> objects;
	vector<DungeonCreature*> creatures;

};
struct DungeonExit
{
	string name;
	string description;	
	DungeonRoom* room;
};

enum USE_EFFECT
{
	HEAL,
	DAMAGE,
	TELEPORT,
	TEXT,
	CREATE_OBJECT,
	CREATE_CREATURE,
	DELETE,
	SHRINK,
	GROW,
	LIGHTER,
	HEAVIER,
	CHANGE,
	NONE,
	NOT_ALLOWED
};

struct DungeonObject
{
	string name;
	string description;
	
	int damage;  //0 if not a weapon
	vector<string> hitMessages;
	vector<string> missMessages;

	int mass;
	int size;
	
	bool container;
	bool open;
	vector<DungeonObject*> contents;	
	string openMessage;
	string closeMessage;
	
	bool takeable;
	string takeMessage;
	string dropMessage;

	vector<string> useAliases;

	USE_EFFECT selfEffect;
	USE_EFFECT creatureEffect;
	USE_EFFECT objectEffect;

};

struct DungeonCreature
{
	string name;
	string description;
	int hitpoints;
	int alignment;
};

struct DungeonPlayer
{
	string name;
	string description;
	int hitpoints;
	int score;
	vector<DungeonObject*> objects;
	DungeonRoom* location;
};

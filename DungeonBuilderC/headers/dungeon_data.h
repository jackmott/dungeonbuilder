#pragma once

#include <string>
#include <vector>

using namespace std;


struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;

struct DungeonRoom
{
	DungeonRoom();
	~DungeonRoom();

	int uid;
	string name;
	vector<string> description;
	vector<DungeonExit*> exits;
	vector<DungeonObject*> objects;
	vector<DungeonCreature*> creatures;

	string toJSON();

};
struct DungeonExit
{
	DungeonExit();
	~DungeonExit();

	string name;
	vector<string> description;	

	bool isDoor;   // or window, or pile of rocks, whatever	
	bool isOpen;   //or whatever

	int distance;

	string openText;
	string closedText;


	DungeonRoom* room;

	string toJSON();
};

enum class USE_EFFECT
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
	DungeonObject();
	~DungeonObject();
	string name;
	vector<string> description;
	
	int damage;  //0 if not a weapon
	
	int mass;
	int size;
	
	bool canOpen;
	bool isOpen;
	
	vector<DungeonObject*> contents;	
	
	bool canTake;
	
	vector<string> useAliases;

	USE_EFFECT selfEffect;
	USE_EFFECT creatureEffect;
	USE_EFFECT objectEffect;


	string toJSON();
};

struct DungeonPlayer
{
	DungeonPlayer();
	~DungeonPlayer();
	string name;
	vector<string> description;
	int hitpoints;
	int score;
	vector<DungeonObject*> objects;
	DungeonRoom* location;
};

struct DungeonCreature
{
	DungeonCreature();
	~DungeonCreature();
	string name;
	vector<string> description;
	int hitpoints;
	int alignment;
	
	string attack(DungeonObject *weapon, DungeonPlayer *player);
	
	
	string toJSON();
};


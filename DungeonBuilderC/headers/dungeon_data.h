#ifndef DUNGEON_DATA
#define DUNGEON_DATA

#include <string>
#include <vector>


using namespace std;



struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;

struct DungeonEntity
{
private:
	vector <string> names;

public:
	DungeonEntity* parent = nullptr;
	unsigned long uid = NULL;	
	virtual void addName(string name);
	virtual void setPrimaryName(string name);
	virtual string getPrimaryName() const;
	virtual vector<string> getNames() const;
	virtual vector<string> getLcaseNames() const;
	virtual string vectorStringToJSON(vector<string> *v) const;
	bool removeName(string name);
};

struct DungeonRoom: DungeonEntity
{
	DungeonRoom();
	~DungeonRoom();

	
	vector<string> description;
	vector<DungeonExit*> exits;
	vector<DungeonObject*> objects;
	vector<DungeonCreature*> creatures;

	string toJSON();

};

extern vector<DungeonRoom*> g_roomList;

struct DungeonExit: DungeonEntity
{
	DungeonExit();
	~DungeonExit();

	

	bool isDoor;   // or window, or pile of rocks, whatever	
	bool isOpen;   //or whatever

	int distance;

	string openingText;
	string closingText;

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
	REMOVE,
	SHRINK,
	GROW,
	LIGHTER,
	HEAVIER,
	CHANGE,
	NONE,
	NOT_ALLOWED
};


struct DungeonObject: DungeonEntity
{
	DungeonObject();
	~DungeonObject();

	vector<string> description;

	int damage;  //0 if not a weapon
	int mass;
	int size;

	bool canOpen;
	bool isOpen;
	bool canTake;

	
	vector<DungeonObject*> contents;
	vector<string> useAliases;
	
	string toJSON();
};

struct DungeonPlayer: DungeonEntity
{
	DungeonPlayer();
	~DungeonPlayer();

	vector<string> description;
	int hitpoints;
	int score;
	vector<DungeonObject*> objects;
	DungeonRoom* location;
};

struct DungeonCreature: DungeonEntity
{
	DungeonCreature();
	~DungeonCreature();

	vector<string> description;
	int hitpoints;
	int alignment;

	string attack(DungeonObject *weapon,DungeonPlayer *player);


	string toJSON();
};

#endif
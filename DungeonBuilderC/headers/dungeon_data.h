#ifndef DUNGEON_DATA
#define DUNGEON_DATA

#include <string>
#include <vector>


using namespace std;



struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonAction;


extern vector<DungeonRoom*> g_roomList;



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
	virtual string vectorStringToJSON(vector<string> const &v) const;
	bool removeName(string name);
};



const string EFFECT_STRS[3] = {
	"Heal","Damage","Transform"};
const enum class EFFECT_TYPE{
	HEAL = 0,
	DAMAGE = 1,
	TRANSFORM =2,
	FIRST = HEAL,
	LAST = TRANSFORM };

struct DungeonEffect
{
	DungeonEffect();
	~DungeonEffect();

	
	DungeonAction *parent;
	EFFECT_TYPE type;
	string output;

	int magnitude;
	DungeonObject *transform;

	void apply(DungeonPlayer* player);
	
	string getName();
};

struct DungeonAction : DungeonEntity
{
	string output;
	vector<DungeonEffect*> effects;
};

struct DungeonRoom : DungeonEntity
{
	DungeonRoom();
	~DungeonRoom();

	
	vector<string> description;
	vector<DungeonExit*> exits;
	vector<DungeonObject*> objects;
	vector<DungeonCreature*> creatures;

	string toJSON();

};



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
	vector<DungeonAction*> actions;
	
	string toJSON();
};

struct DungeonPlayer: DungeonEntity
{
	DungeonPlayer();
	~DungeonPlayer();

	vector<string> description;
	int hitpoints;
	int maxhitpoints;
	int score;
	vector<DungeonObject*> objects;
	DungeonRoom* location;

	void heal(int amount);
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
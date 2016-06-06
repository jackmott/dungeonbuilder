#ifndef DUNGEON_ENTITY_H
#define DUNGEON_ENTITY_H

#include <vector>
#include <string>


struct DungeonEntity
{
private:
	std::vector <std::string> names;

public:
	DungeonEntity* parent = nullptr;
	unsigned long uid = NULL;	
	int age=0;
	virtual void addName(std::string name);
	virtual void setPrimaryName(std::string name);
	virtual std::string  getPrimaryName() const;
	virtual std::vector<std::string> getNames() const;
	virtual std::vector<std::string> getLcaseNames() const;
	virtual std::string  vectorStringToJSON(std::vector<std::string> const &v) const;
	bool removeName(std::string name);
};

#endif

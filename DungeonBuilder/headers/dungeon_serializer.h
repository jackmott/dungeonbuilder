#ifndef DUNGEON_SERIALIZER_H
#define DUNGEON_SERIALIZER_H

#include <string>
#include <fstream>
#include <stack>
#include <vector>


struct DungeonEntity;

//reflection, the C way
// #x expands to a string literal of the variable name
#define writeInt(x) _writeInt(#x,x)
#define writeEnum(x) _writeInt(#x,(int)x)
#define writeString(x) _writeString(#x,x)
#define writeBool(x) _writeBool(#x,x)
#define writeEntity(x) _writeEntity(#x,x)
#define writeVectorString(x) _writeVectorString(#x,x)
//only the ids!
#define writeVectorEntity(x) _writeVectorEntity(#x,&x)

#define loadInt(x,json) x = _loadInt(#x,json)
#define loadEnum(x,json) _loadInt(#x,json)
#define loadString(x,json) x = _loadString(#x,json)
#define loadBool(x,json) x = _loadBool(#x,json)
#define loadVectorString(x,json) x = _loadVectorString(#x,json)

#define loadVectorEntity(x,json) _loadVectorEntity(#x,&x,json)
#define loadEntity(x,json) _loadEntity(#x,(DungeonEntity**)&x,json)


//save
std::string _writeInt(const std::string &name, int value);
std::string _writeString(const std::string &name,std::string value);
std::string _writeBool(const std::string &name,bool value);
std::string _writeVectorString(const std::string &name,std::vector<std::string> const &value);
//only the ids!
std::string _writeEntity(const std::string &name, DungeonEntity* e);
std::string _writeVectorEntity(const std::string &name, void *value);

//load
int _loadInt(const std::string &name, void* json);
std::string _loadString(const std::string &name, void* json);
bool _loadBool(const std::string &name, void* json);
std::vector<std::string> _loadVectorString(const std::string &name, void* json);
void _loadVectorEntity(const std::string &name,void* _v,void *_json);
void _loadEntity(const std::string &name,DungeonEntity **e, void *_json);
void loadJson(std::string filename);

#endif
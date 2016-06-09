#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>


struct DungeonEntity;
struct DungeonObject;
struct DungeonRoom;
struct DungeonPlayer;

extern unsigned long global_id;

#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

//Used by ENUM generating macros
//To produce enums and string arrays
//that are kept in synch
#define GEN_ENUM(ENUM) ENUM,
#define GEN_STRING(STRING) #STRING,


unsigned long getUID();
std::vector<std::string> &split(const std::string  &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string  &s, char delim);
std::string join(unsigned int offset, std::vector<std::string> const &v, std::string  delim);
std::vector<std::string> removeArticles(std::vector<std::string> words);
bool containsWith(std::vector<std::string> word);
std::string toLower(std::string s);
void toLower(std::string *s);
bool isVowel(char c);
std::string a_an (std::string s);
bool isAffirmative(std::string s);
std::string thereIsA(std::string thing);
DungeonEntity* extractEntity(void* _entities ,std::string *userInput, int *matchedName = nullptr);
DungeonObject* extractObject(std::vector<DungeonObject*> * objects,std::string *userInput, int* matchedName = nullptr);
DungeonObject* extractAndRemoveObject(std::vector<DungeonObject*> * objects,std::string *userInput);

void recursiveObjectAccumulator(std::vector<DungeonObject*> *result,std::vector<DungeonObject*> *objects);
std::vector<DungeonObject*> getAllPlayerObjects(DungeonPlayer* player);
std::vector<DungeonObject*> getAllRoomObjects(DungeonRoom* room);
std::vector<DungeonObject*> getAllObjects(DungeonPlayer* player,DungeonRoom* room);

void removePointer(void * _pointers,void* pointer);
bool removeStr(std::vector<std::string> *strs,std::string str);
void strlensort(std::vector<std::string>* v);
void entitysort(std::vector<DungeonEntity*> *v);
std::string extractPhrase(std::vector<std::string> phrasesToFind, std::string  *userInput);
void dbsleep(unsigned int milliseconds);
int min(int a, int b);
int max(int a, int b);
#endif
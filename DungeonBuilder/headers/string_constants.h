#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H




//All(ish)
#define STR_MENU_NAME "[Add/Del/Set](Name):"
#define STR_MENU_DESCRIPTION "[Edit](Desc):"

//Room
#define STR_MENU_ROOM_NAME "[Set](Name):"
#define STR_MENU_OBJECT "[Add/Del/Edit](Object):"
#define STR_MENU_CREATURE "[Add/Del/Edit](Creature):"
#define STR_MENU_EXIT "[Add/Del/Edit](Exit):"
#define STR_MENU_ROOM_MOVE "[Go](Exit/Blank):"
#define STR_MENU_HAS_LIGHT "[Light](T/F):"

//Creature
#define STR_MENU_HITPOINTS "[Set](Hitpoints):"
#define STR_MENU_ALIGNMENT "[Set](Alignment):"

//Exit
#define STR_MENU_EXIT_ROOM "[Set/Edit](Room):"


//Objects
#define STR_MENU_DURABILITY "[Set](Durability):"
#define STR_MENU_ACTIONS "[Add/Del/Edit](Action):"
#define STR_MENU_TRIGGERS "[Add/Del/Edit](Trigger):"
#define STR_MENU_TAKEABLE "[Set](Takeable)[T/F]:"
#define STR_MENU_CAN_OPEN "[Set](Openable)[T/F]:"
#define STR_MENU_IS_OPEN "[Set](Open)[T/F]:"
#define STR_MENU_IS_DOOR "[Set](Door)[T/F]:"
#define STR_MENU_OPENTEXT "[Set](Desc/OpenText):"
#define STR_MENU_CLOSEDTEXT "[Set](ClosedText):"
#define STR_MENU_OPENINGTEXT "[Set](OpeningText):"
#define STR_MENU_CLOSINGTEXT "[Set](ClosingText):"

//Room List
#define STR_MENU_ID_OR_NEW "[#/New](Name):"

//Effect
#define STR_MENU_TYPE "[Set](Type):"
#define STR_MENU_MAGNITUDE "[Set](Magnitude)[#]:"
#define STR_MENU_SET_ADD_OBJECT "[Set/Add/Del/Edit](Object):"
//Action
#define STR_MENU_TEXT_OUTPUT "[Set](Output):"
#define STR_MENU_EFFECT "[Add/Del/Edit #](Effect):"
#define STR_MENU_NEED_HOLD "[Set](Held):"


#define STR_DURABILITY "durability"
#define STR_LIGHT "light"
#define STR_NEED_HELD "held"
#define STR_TYPE "type"
#define STR_MAGNITUDE "magnitude"
#define STR_TEXT_OUTPUT "output"
#define STR_EFFECT "effect"
#define STR_GO "go"
#define STR_NEW "new"
#define STR_INVENTORY "inventory"
#define STR_I "i"
#define STR_CREATE "create"
#define STR_OPEN_TEXT "opentext"
#define STR_CLOSED_TEXT "closedtext"
#define STR_OPENING_TEXT "openingtext"
#define STR_CLOSING_TEXT "closingtext"
#define STR_DOOR "door"
#define STR_EXIT "exit"
#define STR_SET "set"
#define STR_EDIT "edit"
#define STR_OBJECT "object"
#define STR_CREATURE "creature"
#define STR_DESCRIPTION "description"
#define STR_DESC "desc"
#define STR_NAME "name"
#define STR_ACTION "action"
#define STR_TRIGGER "trigger"
#define STR_ADD "add"
#define STR_DELETE "del"
#define STR_ENTER "enter"
#define STR_SAVE "save"
#define STR_LOAD "load"
#define STR_HITPOINTS "hitpoints"
#define STR_ALIGNMENT "alignment"
#define STR_ROOM "room"
#define STR_USE "use"
#define STR_DAMAGE "damage"
#define STR_TRUE "Yes"
#define STR_FALSE "No"
#define STR_TAKEABLE "takeable"
#define STR_OPENABLE "openable"
#define STR_TAKE "take"
#define STR_LOOK "look"
#define STR_OPEN "open"
#define STR_CLOSE "close"
#define STR_PUT "put"
#define STR_PLACE "place"
#define STR_LOOK_AT "look at"
#define STR_EXAMINE "examine"
#define STR_DROP "drop"
#define STR_PAGE_UP "page_up"
#define STR_PAGE_DOWN "page_down"
#define STR_KEY_RESIZE "curses_resize"


#define STR_QUOT "\""
#define STR_TAB  "    "
#define STR_RIGHT_ARROW " \xAF "
#define STR_JOINER ","
#define STR_PROMPT ">"
#define STR_ELLIPSES "..."

#ifdef _WIN32
#define CHR_NEWLINE 170
#else
#define CHR_NEWLINE 172 //This doesn't seem to work in common terminals. bla.
#endif
#define CHR_SPACE ' '
#define STR_JSON_TRUE "1"


#endif

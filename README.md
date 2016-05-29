# Dungeon Builder
First person text based N dimensional adventure engine for all platforms

## Status
Builds and Runs in Windows (using pdcurses)
Builds and Runs in Linux (using ncurses 6.0) but has issues with colors and key code matching
Builds and Runs on OSX

At this point you can create arbitrarily large directed cyclic graphs of 'rooms', put creatures and objects in the 'rooms', and kill those creatures with those objects. Objects can go inside of other objects. You can take them out, put them back in, examine them, and drop them.

## How To Help
contact me at jack.mott@theemailmanypeopleusethatgoogledoes
### Things To Do
- General C/C++ guidance 
- Fixing *NIX specific issues 
- Setting up a system where game state can be tranmitted via sockets - allowing multi-term/monitor support
- An editor screen to list all creatures/rooms/objects with some sort of interface for selction by id?
- Feature to save game transcript to a file
- Enhance the in game text editor
- A markup system for 'bold' and color
- A system to 'copy' an existing object to speed creation of another similar or identical one.
- A more graceful system of responses to failed and successful commands. Perhaps randomized.
- A combat system
- A generalized object use/effect/crafting system (drink a glass of water -> get effect -> get empty bottle)


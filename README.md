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
- Better handling of language. Accurate selection of articles "a / an / some", auto-plural forms etc.
- Setting up a system where game state can be tranmitted via sockets - allowing multi-term/monitor support
- Feature to save game transcript to a file
- Preview mode for in game text editor
- A system to 'copy' an existing object to speed creation of another similar or identical one.
- A more graceful system of responses to failed and successful commands. Perhaps randomized.
- A combat system
- Some interface for easier creation of 'two way' exits.
- Expanding on the available set of effects and triggers


# DOSBox-custom

In this directory are interface files for connecting the DOSBox emulator
with self written/reverse engineered code.
At the moment the loading and termination of executables are supportet.
Furthermore the function calls of the CPU are passed into the code of the software under investigation
and can _easily_ be modified to replace emulated with rewritten code.

Since ancient software can be available in different versions, it seems a good idea to
put this logic into the corrospnding subdirectories, for example "schick" and "schweif".

The directory "drive_c" is a DOSBox environment for recompiling rewritten code to avoid errors and mistakes.

## Future Plans
* Detect known Compilers/Memory Models in a generalized way
* Some Executables are compressed (maybe detect compression algo)


## Possible Games
Borland C++: "Spirit of Adventure", "Conan the Cimmerian", "Darkseed1", "Dungeon Hack", ...



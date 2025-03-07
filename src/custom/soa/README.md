# SPIRIT OF ADVENTURE

In this directory are files for the support of "SPIRIT OF ADVENTURE",
the predecessor of "BLADE OF DESTINY". And guess what: there are similiarities.

## USAGE

### Prequisites
1. have an ENGLISH copy of the game available installed in a DOSBox environment
2. make copy the file VGAME and name it VGAME.EXE (VGA Version of the game)
3. use VGAME.EXE to start the game

To verify everything runs as expected, the filesize is 185.356 bytes:

    md5sum VGAME.EXE
    9836f184abc5ac5779c41bb3133abb0e  VGAME.EXE
    shasum VGAME.EXE
    52103cd4c33bb6bde5c4001d5c256a9e96efdf22  VGAME.EXE


## Technical Information
* was build with the famous Borland C++ compiler (v2.0, Memory Model Large, Dataseg != Stack)
* doesn't use overlay technique as used in the much larger SCHICKM.EXE
* doesn't have a single datafile (49 datafiles + 1 Savegame + 1 Executable)

## Goals
* Have a more direct approach
* no support for BIG-ENDIAN Machines
* checks from the begining

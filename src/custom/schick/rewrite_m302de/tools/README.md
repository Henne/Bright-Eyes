# tools folder (game specific)

In this directory are small helpers to prepare manual and automated testing.

## Usage
### Preparation (disassemble.sh + nc2.fc.py)
This needs only to be done once, but it needs to be done the right way.
0. make sure you have installed: Nasm, Python3, DOSBox
1. place an executable into this directory, e.g. GAME.EXE
2. edit disassemble.sh (GAMEFILE, offsets/lengths of EACH segment inside GAME.EXE) MAGIC!!!
3. run it, check in ../temp/disasm_orig for the disassembly files

### Manual check (bc.sh + dump_obj)
This should be used while working on a specific segment.
Requires a working compilation environment!
0. go into the rewrite directory of GAME.EXE
1. edit compile.bat to select the segment to investigate
2. run ./tools/bc.sh from there sucessfully
3. investigate output

### Automatic test (bc_ready.sh + dump_obj)
#### preparation 
Requires a working compilation environment!
* edit Bright-Eyes/.git/hooks/pre-commit to run bc_ready.sh
* make a commit

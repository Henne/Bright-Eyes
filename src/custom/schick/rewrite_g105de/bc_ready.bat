@ECHO OFF
@REM These are the files, which produce the same code as the originals.
@REM They shall not change, so the are compared with every commit.
@REM
@REM -- here start the complete
BCC.EXE -mlarge -O- -c -2 SEG001.CPP
BCC.EXE -mlarge -O -c -2 -IAIL SEG002.CPP
BCC.EXE -mlarge -O2 -c -1 -Y SEG003.CPP
TASM.EXE /os /z SEG004.asm SEG004.OBJ
TASM.EXE /os /z SEG005.asm SEG005.OBJ
TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM SEG006.OBJ
TASM.EXE /os /z SEG007.asm SEG007.OBJ
TLINK @TLINK.RES

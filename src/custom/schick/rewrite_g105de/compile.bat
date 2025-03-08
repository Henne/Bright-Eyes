@ECHO OFF
@REM -G Optimize speed, -G- optimize size (default)
@REM -O1 Smallest Code -O -Ob -Oe -Os -k- -Z
@REM -O2 Fastest Code -O -Ob -Oe -Og -Oi -Ol -Om -Op -Ot -Ov -k- -Z
@REM -j5 = #Errors
@REM
@REM --the hard cases
@REM -Os, -Od, -k- geht auch
@REM option -P can be enabled explicit	(force C++ compilation)
@REM option -k can be enabled explicit	(standart stack frame), but -k- also works
@REM option -a- can be enabled explicit	(disable word alignment)
@REM option -mlarge or -ml! are allowed	(memory model large with or without DSSTACK)
@REM option -K must be disabled 	(default char is unsigned), but this max be changed in the code
@REM option -1 can be enabled explicit	(80186 instructions), -2 also works, but never disable it with -1-
@REM option -Fm is not enabled		(shortcut for -Fc, -Ff and -Fs)
@REM option -Fc can be enabled explicit	(generate communal variables)
@REM option -Ff must be disabled	(generate far variables)
@REM option -Fs must be disabled	(assume ss == ds), but only 4 files are affected
@REM option -O is not enabled
@REM option -Od can be enabled		(disable optimization)
@REM
@REM -- here start the complete
@REM BCC.EXE -mlarge -O- -c -Y SEG001.CPP
@REM BCC.EXE -mlarge -O- -c -Y SEG002.CPP
@REM BCC.EXE -mlarge -O- -c -Y SEG003.CPP
@REM BCC.EXE -mlarge -O- -c -Y SEG004.CPP
@REM BCC.EXE -mlarge -O- -c -Y SEG005.CPP
@REM BCC.EXE -mlarge -O- -c -Y SEG006.CPP
@REM ..\BIN\TASM.EXE /os /z SEG013.asm SEG013.OBJ

@REM examine the ouput and press anykey
PAUSE

@ECHO OFF
@REM -G Optimize speed, -G- optimize size (default)
@REM -O1 Smallest Code -O -Ob -Oe -Os -k- -Z
@REM -O2 Fastest Code -O -Ob -Oe -Og -Oi -Ol -Om -Op -Ot -Ov -k- -Z
@REM
@REM
@REM ..\BIN\BCC.EXE -mlarge -Od -c SEG002.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG024.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG027.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- .c -1 -Y SEG053.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG072.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG036.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG037.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y -j5 SEG044.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y -j5 SEG100.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y -j5 SEG101.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG108.CPP
@REM
@REM -- ready some work todo
@REM ..\BIN\BCC.EXE -mlarge -O -c -1 -Y- SEG006.CPP
@REM
@REM --here start the good ones --
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG120.CPP
@REM -- here start the complete
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG047.CPP

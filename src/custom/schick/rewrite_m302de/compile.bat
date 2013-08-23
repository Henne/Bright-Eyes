@ECHO OFF
@REM -O1 Smallest Code -O -Ob -Oe -Os -k- -Z
@REM -O2 Fastest Code -O -Ob -Oe -Og -Oi -Ol -Om -Op -Ot -Ov -k- -Z
@REM
@REM
@REM ..\BIN\BCC.EXE -mlarge -Od -c SEG002.CPP
@REM ..\BIN\BCC.EXE -mlarge -1 -O- -Ot -j3 -c SEG006.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -j3 -c SEG027.CPP
@REM..\BIN\BCC.EXE -mlarge -1 -O- -Y -c SEG053.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -Y -c SEG072.CPP
@REM ..\BIN\BCC.EXE -mlarge -Od -c -Y SEG036.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -k+ -1 -Y SEG037.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -k+ -1 -Y -j5 SEG044.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -k+ -1 -Y -j5 SEG100.CPP
@REM ..\BIN\BCC.EXE -mlarge -Od -c -Y SEG108.CPP
@REM
@REM
@REM --here start the good ones --
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG120.CPP
@REM -- here start the complete
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG047.CPP

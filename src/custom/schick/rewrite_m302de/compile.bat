@ECHO OFF
@REM -G Optimize speed, -G- optimize size (default)
@REM -O1 Smallest Code -O -Ob -Oe -Os -k- -Z
@REM -O2 Fastest Code -O -Ob -Oe -Og -Oi -Ol -Om -Op -Ot -Ov -k- -Z
@REM -j5 = #Errors
@REM
@REM --the hard cases
@REM -Os, -Od, -k- geht auch
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG106.CPP
@REM
@REM --the compiling cases
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG001.CPP
@REM
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y -j5 SEG009.CPP
@REM
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG048.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG075.CPP
@REM
@REM -- here start the complete
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y -IAIL SEG002.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG003.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG004.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG005.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG006.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Y SEG007.CPP
@REM ..\BIN\TASM.EXE /os /z SEG008.asm SEG008.OBJ
@REM ..\BIN\TASM.EXE /os /z SEG009.asm SEG009.OBJ
@REM ..\BIN\TASM.EXE /os /z SEG010.asm SEG010.OBJ
@REM ..\BIN\TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM SEG011.OBJ
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG024.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG025.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG026.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG027.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG028.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG029.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG030.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG031.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG032.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG033.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG034.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG035.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG036.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG037.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG038.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG039.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG040.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG041.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG042.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG043.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG044.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG045.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG046.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG047.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG049.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG050.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG051.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG052.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG053.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG054.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG055.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG056.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG057.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG058.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG059.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG060.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG061.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG062.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG063.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG064.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG065.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG066.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG067.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG068.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG069.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG070.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG071.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG072.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG073.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG074.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG076.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG080.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG081.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG086.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG087.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG088.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG090.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG091.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG092.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG093.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG094.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG095.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG096.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG097.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG098.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG099.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG100.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG101.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG102.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG103.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG104.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG105.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG107.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG108.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG109.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG110.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG111.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG112.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG113.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG114.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG115.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG116.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG117.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG118.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG119.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG120.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG121.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG122.CPP
@REM -- here start the empty
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG077.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG078.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG079.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG082.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG083.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG084.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG085.CPP
@REM ..\BIN\BCC.EXE -mlarge -O- -c -1 -Yo SEG089.CPP

@REM examine the ouput and press anykey
PAUSE

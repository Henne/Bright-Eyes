# DSA1/RAK1 Character generator

## Versions
All versions are build with the Borland C++ compiler using Memory Model: LARGE.
| Version | Language | Compiler Version | Size  | md5sum                           |
| ------- | -------- | ---------------- | ----  | -------------------------------- |
| V1.00   | DE       | 2.0              | 57352 | 03a78b1fa703db58bf93238c81d7bdd9 |
| V1.03   | DE       | 2.0              | 57384 | 51474416de1c84de2d17bd819355d92c |
| V1.04   | DE       | 3.1              | 57854 | e1cf2251c9217a8992129f3dc42167df |
| V1.05   | DE       | 3.1              | 74112 | 87be80fb584479d96c4dce3c2fac6eb7 |
| V3.00   | EN       | 3.1              | 61012 | 10e7f0372c96b467dbde2b60477d61eb |


## Layout V1.00 - V1.04 DE
| Segment | #Funcs | Content |
| ------- | ------ | ------- |
| seg000  |      - | C-Lib Codesegment |
| seg001  |    123 | Charactergenerator (mostly everything) |
| seg002  |      4 | random number generator |
| seg003  |     23 | SOUND (NOT AIL) |
| seg004  |     24 | Rasterlib (VGA) |
| seg005  |      7 | PP20 decompressor (for compressed images) |
| seg006  |      - | Datasegment |

## Layout V1.05 DE
| Segment | #Funcs | Content |
| ------- | ------ | ------- |
| seg000  |      - | C-Lib Codesegment |
| seg001  |     21 | CD-Audio Code |
| seg002  |    112 | Charactergenerator (mostly everything) |
| seg003  |      4 | random number generator |
| seg004  |      7 | PP20 decompressor (for compressed images) |
| seg005  |     24 | Rasterlib (VGA) |
| seg006  |      - | AIL (Sound) |
| seg007  |      - | DATA for CD-Audio |
| seg008  |      - | Datasegment |

## Layout V3.00 EN
| Segment | #Funcs | Content |
| ------- | ------ | ------- |
| seg000  |      - | C-Lib Codesegment |
| seg001  |      ? | Charactergenerator (mostly everything) |
| seg002  |      4 | random number generator |
| seg003  |      7 | PP20 decompressor (for compressed images) |
| seg004  |     24 | Rasterlib (VGA) |
| seg005  |      - | AIL (Sound) |
| seg006  |      - | Datasegment |


## DSAGEN.DAT Versions

There are only 3 different versions of DSAGEN.DAT.

| Used by | Size   | md5sum                           | Notes                                     |
| ------- | ------ | -------------------------------- | ----------------------------------------- |
| DE DISK | 634785 | 0731d036ac4a3240e7058639bfff4e3d | contains GEN.AWS                          |
| DE CD   | 663221 | e713a3a24436925609a9dcf4c047df07 | contains GEN.XMI, SAMPLE.AD, MT32EMUL.XMI |
| EN DISK | 671236 | 07e7297c1465388af8df689c0743bb03 | contains former 3 files + ROALOGUS.DAT    |

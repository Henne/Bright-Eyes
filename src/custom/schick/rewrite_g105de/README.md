# DSA1/RAK1 Character generator

## GEN.EXE Versions
All versions are build with the Borland C++ compiler using Memory Model: LARGE.
| Version | Language | Compiler Version | Size  | md5sum                           |
| ------- | -------- | ---------------- | ----  | -------------------------------- |
| V1.00   | DE       | 2.0              | 57352 | 03a78b1fa703db58bf93238c81d7bdd9 |
| V1.03   | DE       | 2.0              | 57384 | 51474416de1c84de2d17bd819355d92c |
| V1.04   | DE       | 3.1              | 57854 | e1cf2251c9217a8992129f3dc42167df |
| V1.05   | DE       | 3.1              | 74112 | 87be80fb584479d96c4dce3c2fac6eb7 |
| V3.00   | EN       | 3.1              | 61012 | 10e7f0372c96b467dbde2b60477d61eb |


### Layout V1.00 - V1.04 DE
| Segment | #Funcs | Content |
| ------- | ------ | ------- |
| seg000  |      - | C-Lib Codesegment |
| seg001  |    123 | Charactergenerator (mostly everything) |
| seg002  |      4 | random number generator |
| seg003  |     23 | SOUND (NOT AIL) |
| seg004  |     24 | Rasterlib (VGA) |
| seg005  |      7 | PP20 decompressor (for compressed images) |
| seg006  |      - | Datasegment |

### Layout V1.05 DE
| Segment | #Funcs | Content |
| ------- | ------ | ------- |
| seg000  |      - | C-Lib Codesegment |
| seg001  |     21 | CD-Audio Code |
| seg002  |    132 | Charactergenerator (mostly everything) |
| seg003  |      4 | random number generator |
| seg004  |      7 | PP20 decompressor (for compressed images) |
| seg005  |     24 | Rasterlib (VGA) |
| seg006  |      - | AIL (Sound) |
| seg007  |      - | DATA for CD-Audio |
| seg008  |      - | Datasegment |

### Layout V3.00 EN
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


### Content

Remark: The index here is NOT same as in GEN.EXE!

| Index | DE (CD / DISK)    | md5sum                           | EN DISK      | md5sum                           | Notes |
| ----- | ----------------- | -------------------------------- | ------------ | -------------------------------- | ----- |
| 00    | DSALOGO.DAT       | 7dec8a345070fe6026ef23ecb78dbf9c | ROALOGUK.DAT | 40a6f05ee336b4a4f5fecb563d003f76 |       |
| 01    | GEN1.NVF          | fc93e1d82cce58cb898ac330e952b4d2 | E_GEN1.NVF   | 1be496c698280668e3d02816da397d2b |       |
| 02    | GEN2.NVF          | b83749a7e95934f76ad86eb2adf183b8 | E_GEN2.NVF   | cd22c8e2c47aa3265c00d4d6c23c5f78 |       |
| 03    | GEN3.NVF          | de68b7c7dd95c8ee20ae3e0fe97b1851 | E_GEN3.NVF   | eafde52b724dcaff9adbc67966e112c7 |       |
| 04    | GEN4.NVF          | 36c59acf1e86d631fcbe3cf0baff7e51 | E_GEN4.NVF   | 448a3f79362e258bf96108264bb7d5ed |       |
| 05    | GEN5.NVF          | 19bee75935ea8476b11f2bdc71e87b68 | E_GEN5.NVF   | b0aacb745583304f62669fa71915fe67 |       |
| 06    | GEN6.NVF          | f599eb439b3a9f4292530ac09d04d1eb | E_GEN6.NVF   | 095eb1f93dfb6ed94ee369ea60964f47 |       |
| 07    | GEN7.NVF          | 6bd80e091b49446c7da977e8e9c75630 | E_GEN7.NVF   | 87b3e13e7220a92c416c4922f1386a80 |       |
| 08    | GEN8.NVF          | 731ce148cc2933937c5fae07fb48a54d | E_GEN8.NVF   | 602b739cbb051427060a0fdaa7930958 |       |
| 09    | GEN9.NVF          | e8a253b0d3b81cfb08154de25a92c1a8 | E_GEN9.NVF   | 4b471196489eed8f49deed59ac01f37c |       |
| 10    | GEN10.NVF         | c196accb01602bff31ddcbbe70412aa8 | E_GEN10.NVF  | 8ccdd17762e5cb4a4cbd387b8f6f4b88 |       |
| 11    | GEN11.NVF         | 9aaa0e97c7fea04e733d5677a98eafec | E_GEN11.NVF  | 556f3eca04db689bfa5ce6a028d2301b |       |
| 12    | GENTIT.DAT        | 5093052c8dcfffb78736ae52510ff8fd | E_GENTIT.NVF | 0dd7e449bcbc1f235dedfabb665da91c |       |
| 13    | HEADS.DAT         | 6d405fe084cdd515c8bb84f27cf879cb | HEADS.DAT    | 6d405fe084cdd515c8bb84f27cf879cb |       |
| 14    | POPUP.DAT         | a4e2155a9024d4d3aa17ab04a1ada8a8 | POPUP.DAT    | a4e2155a9024d4d3aa17ab04a1ada8a8 |       |
| 15    | SEX.DAT           | 08df2c298ed40ef81b3a0a122b26514b | SEX.DAT      | 08df2c298ed40ef81b3a0a122b26514b |       |
| 16    | DZWERG.DAT        | a05eac70502c5bf4334f7a5f1a043006 | DZWERG.DAT   | a05eac70502c5bf4334f7a5f1a043006 |       |
| 17    | DTHORWAL.DAT      | 4c0aa9456a09a1587572a0c9955a7579 | DTHORWAL.DAT | 4c0aa9456a09a1587572a0c9955a7579 |       |
| 18    | DSTREUNE.DAT      | d097eea9b4e58ceed9918299e20c7094 | DSTREUNE.DAT | d097eea9b4e58ceed9918299e20c7094 |       |
| 19    | DMENGE.DAT        | 3ffad0c5b548774d0be8c6837913b5a1 | DMENGE.DAT   | 3ffad0c5b548774d0be8c6837913b5a1 |       |
| 20    | DMAGIER.DAT       | 195bfb0ed4bebf4e21072ccc66c220aa | DMAGIER.DAT  | 195bfb0ed4bebf4e21072ccc66c220aa |       |
| 21    | DKRIEGER.DAT      | 57ba9fdf08b39bf7a03c3d03297f554f | DKRIEGER.DAT | 57ba9fdf08b39bf7a03c3d03297f554f |       |
| 22    | DDRUIDE.DAT       | f4f461855f51a8f66a5acd5facd8993c | DDRUIDE.DAT  | f4f461855f51a8f66a5acd5facd8993c |       |
| 23    | DAELF.DAT         | 0417f1e1a39fb559f9c6e64569b55b28 | DAELF.DAT    | 0417f1e1a39fb559f9c6e64569b55b28 |       |
| 24    | DFELF.DAT         | d733da8ca44d2b4113f0ee08cbe7a24d | DFELF.DAT    | d733da8ca44d2b4113f0ee08cbe7a24d |       |
| 25    | DWELF.DAT         | f8cd01d6b959bcf0794a400a7a04b0e5 | DWELF.DAT    | f8cd01d6b959bcf0794a400a7a04b0e5 |       |
| 26    | DGAUKLER.DAT      | 0b535f807d0e8c29b5393ef1460352bb | DGAUKLER.DAT | 0b535f807d0e8c29b5393ef1460352bb |       |
| 27    | DHEXE.DAT         | 1fe39c37f5496bd457bdfbfd0c028a4b | DHEXE.DAT    | 1fe39c37f5496bd457bdfbfd0c028a4b |       |
| 28    | DJAEGER.DAT       | 701baaca6c2f7a46fa1f92d796e5c8e9 | DJAEGER.DAT  | 701baaca6c2f7a46fa1f92d796e5c8e9 |       |
| 29    | GENTEXT           | 06c8f19a5453c365b1c71c5491354dc8 | E_GENTXT     | f2fea13f9fc91c42837e3c44e2311755 |       |
| 30    | ATTIC             | 57d5822c8c9ce239fbb88de7c20c38cf | ATTIC        | 57d5822c8c9ce239fbb88de7c20c38cf |       |
| 31    | GEN.XMI (CD)      | 0d994b2db0a09faaa72cb422680f07f8 | GEN.XMI      | 0d994b2db0a09faaa72cb422680f07f8 |       |
| ...   | GEN.AWS (DISK)    | 26d29b1a4589d62fc27df6daae872a47 | -            |                                  |       |
| 32    | FANPRO.NVF        | fdc9d09afb9c92caf5e4a0975d8d66be | FANPRO.NVF   | fdc9d09afb9c92caf5e4a0975d8d66be |       |
| 33    | FONT6             | b3e6447acbdfe48623b290815f59a253 | FONT6        | b3e6447acbdfe48623b290815f59a253 |       |
| 34    | SAMPLE.AD    (CD) | 255848c00be41c609d49864bcbc35ebd | SAMPLE.AD    | 255848c00be41c609d49864bcbc35ebd |       |
| 35    | MT32EMUL.XMI (CD) | b4b822f9d0a33586f44e2a99a2739606 | MT32EMUL.XMI | b4b822f9d0a33586f44e2a99a2739606 |       |
| 36    | ROALOGUS.DAT (CD) | d2db97a542f2bf3c3edba8d89e9730c3 | ROALOGUS.DAT | d2db97a542f2bf3c3edba8d89e9730c3 |       |
| 37    | CREATECA.AWS (CD) | 7562c0dc3cbeac4c322f8e2a5e7d6c46 | CREATECA.AWS | 7562c0dc3cbeac4c322f8e2a5e7d6c46 |       |

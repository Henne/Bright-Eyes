/*
 *	symbol names for version v3.02_de
 *
 *	Here are the mappings from adresses in the datasegment
 *	to symbolic names of global variables.
 *	This will only work with the version v302de.
 *
 *	NOTE: between symbol names and adresses are only spaces allowed! NO TABS!
 */

#if !defined SYMBOLS_H
#define SYMBOLS_H

#define CD_INIT_SUCCESSFUL              (0x0095)    /* unsigned short; {0,1} */
#define CD_AUDIO_REPEAT                 (0x009b)    /* unsigned short; {0,1} */

#if 0
// ?4
#define CD_AUDIO_PAUSED                 (0x00a1)    /* unsigned short; {0,1} */
#define CD_AUDIO_TRACK_BAK              (0x00a3)    /* unsigned short */
#define CD_AUDIO_TRACKLIST              (0x00a5)    /* signed short[15] */
#define CD_CHECK_SKIPMAGIC              (0x00c3)    /* unsigned long */
#define CD_CHECK_ERR_COUNTDOWN          (0x00c7)    /* unsigned short */
#define STR_INSERT_CD                   (0x00c9)    /* char[142]; "BITTE LEGEN SIE DIE \xf2SCHICKSALSKLINGE-CD\xf0 IN LAUFWERK %c: EIN. DIESE WIRD BEN\x99TIGT, DA DATEN W\x8eHREND DES SPIELS VON CD GELADEN WERDEN M\x9aSSEN." */
#define STR_REPEAT                      (0x0157)    /* char[12]; "WIEDERHOLEN" */
#define STR_QUIT                        (0x0163)    /* char[8]; "BEENDEN" */
#define STR_CD_EXEPATH                  (0x016b)    /* char[19]; "X:\DSA\SCHICKM.EXE" */
#define STR_CD_MISSING                  (0x017e)    /* char[204]; "DIESES PROGRAMM IST OHNE CD-ROM-LAUFWERK NICHT LAUFF\x8eHIG. BITTE INSTALLIEREN SIE DIE GER\x8eTETREIBER F\x9aR DAS CD-ROM-LAUFWERK GEM\x8eSS IHRER CD-ROM-DOKUMENTATION UND STARTEN SIE DIE 'SCHICKSALSKLINGE' ERNEUT." */
#define STR_CD_INIT                     (0x024a)    /* char[37]; "INITIALISIERE CD-ROM IN LAUFWERK %c:" */


#define CD_AUDIO_PAUSE_POS              (0xbc38)    /* unsigned long */
#define CD_AUDIO_PAUSE_TOD              (0xbc3c)    /* unsigned long */
// ?8

#endif

#define MOUSE_MASK                      (0x1200)
#define MOUSE_POSY_MIN                  (0x1240)
#define MOUSE_POSX_MIN                  (0x1242)
#define MOUSE_POSY_MAX                  (0x1244)
#define MOUSE_POSX_MAX                  (0x1246)
#define MOUSE_LOCKED                    (0x1248)
#define MOUSE_REFRESH_FLAG              (0x124a)    /* signed short */
#define MOUSE_POSX                      (0x124c)
#define MOUSE_POSY                      (0x124e)

#define MOUSE_MOVED                     (0x1254)
#define MOUSE_POINTER_OFFSETX           (0x1256)
#define MOUSE_POINTER_OFFSETY           (0x1258)

#define DEFAULT_ACTION                  (0x1272)    /* RealPt */
#define ACTION_TABLE                    (0x1276)    /* RealPt */

#define GEN_PAGE                        (0x1324)    /* signed short */

#define USELESS_VARIABLE                (0x1326)    /* Bit8s */

/* struct hero */
#define HERO_NAME                       (0x132c)    /* char[16] */
#define HERO_ALIAS                      (0x133c)    /* char[16] */
#define HERO_TYPUS                      (0x134d)    /* Bit8s */
#define HERO_GROUP                      (0x13b4)    /* Bit8s */
#define HERO_PIC                        (0x1606)    /* char[1024] */


#define MIDI_DISABLED                   (0x1a07)    /* signed short */
#define USE_CDA	                        (0x1a09)    /* signed short */
#define MOUSE_HANDLER_INSTALLED         (0x1a0b)    /* signed short */

#define BG_BUFFER                       (0x1a11)    /* RealPt[11] */
#define BG_LEN                          (0x1a3d)    /* Bit32u[11] */
#define TYPUS_BUFFER                    (0x1a69)    /* RealPt[13] */
#define TYPUS_LEN                       (0x1a9d)    /* Bit32u[13] */
#define FNAMES_G105de                   (0x1ad1)    /* RealPt[37] */
#define STR_FILE_MISSING                (0x1b65)    /* char[19] */

#define STRUCT_COL_WHITE2               (0x1b79)    /* RGB */

#define ACTION_INPUT                    (0x1c63)
#define ACTION_PAGE                     (0x1c79)    /* RealPt[11] */

#define STR_SOUND_CFG                   (0x1dd0)    /* char[9]; "SOUND.CFG" */
#define STR_SOUNDHW_NOT_FOUND           (0x1de4)    /* char[]; "SOUND HARDWARE NOT FOUND" */
#define STR_CHR                         (0x1dfe)    /* char[5]; ".CHR" */

#define STR_TEMP_DIR                    (0x1e03)    /* char[] */
#define STR_SAVE_ERROR                  (0x1e09)    /* char[] */

#define STR_DSAGEN_DAT                  (0x1fb0)    /* char[] */

#define RANDOM_GEN_SEED                 (0x1fd6)    /* Bit16u */

#define CD_AUDIO_TRACK                  (0x245a)    /* unsigned short */
#define CD_AUDIO_TOD                    (0x2464)    /* unsigned long */
#define CD_AUDIO_POS                    (0x2468)    /* unsigned long */
#define CD_DRIVE_NO                     (0x246c)    /* unsigned short */

#define CURRENT_TIMBRE_LENGTH           (0x2474)    /* Bit16u */
#define CURRENT_TIMBRE_PATCH            (0x2476)    /* Bit8s */
#define CURRENT_TIMBRE_BANK             (0x2477)    /* Bit8s */
#define CURRENT_TIMBRE_OFFSET           (0x2478)    /* Bit32u */


#define FLEN                            (0x3f2a)    /* Bit32s */
#define FLEN_LEFT                       (0x3f2e)    /* Bit32s */
#define IRQ78_BAK                       (0x3f32)    /* RealPt */
#define GENDAT_OFFSET                   (0x3f36)    /* Bit32s */
#define HANDLE_TIMBRE                   (0x3f3a)    /* Bit16s */
#define TIMBRE_CACHE_SIZE               (0x3f3c)    /* Bit16s */
#define STATE_TABLE_SIZE                (0x3f3e)    /* Bit32u */
#define SND_DRIVER                      (0x3f42)    /* RealPt */
#define FORM_XMID                       (0x3f46)    /* RealPt */
#define SND_PTR_UNKN1                   (0x3f4a)    /* RealPt */
#define STATE_TABLE                     (0x3f4e)    /* RealPt */

#define SND_SEQUENCE                    (0x3f5a)    /* Bit16s */
#define SND_DRIVER_HANDLE               (0x3f5c)    /* Bit16s */

#define CALLED_WITH_ARGS                (0x3f60)    /* Bit16s */

#define HEAD_CURRENT                    (0x40b6)    /* Bit8s */

#define MENU_TILES                      (0x40b9)    /* Bit16s */

#define DST_DST                         (0x40c1)    /* RealPt => PhysPt*/
#define DST_X1                          (0x40c5)    /* Bit16s */
#define DST_Y1                          (0x40c7)    /* Bit16s */
#define DST_X2                          (0x40c9)    /* Bit16s */
#define DST_Y2                          (0x40cb)    /* Bit16s */
#define DST_SRC                         (0x40cd)    /* RealPt */
#define UNKN1                           (0x40d1)    /* Bit16s */
#define UNKN2                           (0x40d3)    /* Bit16s */
#define UNKN3                           (0x40d5)    /* Bit16s */
#define UNKN4                           (0x40d7)    /* Bit16s */
#define TEXTS                           (0x40d9)    /* RealPt[285] array */

#define RANDOM_GEN_SEED2                (0x458f)    /* Bit16u */

#define WO_VAR                          (0x4595)    /* Bit16u */

#define IN_KEY_ASCII                    (0x459d)    /* Bit16s */
#define IN_KEY_EXT                      (0x459f)    /* Bit16s */

#define HAVE_MOUSE                      (0x4591)    /* Bit16s */

#define MOUSE1_EVENT1                   (0x4597)    /* Bit16s */
#define MOUSE2_EVENT                    (0x4599)    /* Bit16s */
#define MOUSE1_EVENT2                   (0x459b)    /* Bit16s */

#define ARRAY_1                         (0x45a1)    /* char[64] */
#define ARRAY_2                         (0x45e1)    /* char[64] */

#define MOUSE_LAST_CURSOR               (0x4621)
#define MOUSE_CURRENT_CURSOR            (0x4625)

#define MOUSE_BACKBUFFER                (0x4669)
#define BUFFER_SEX_DAT                  (0x4769)    /* RealPt SEX.DAT */
#define BUFFER_POPUP                    (0x476d)    /* RealPt POPUP.NVF */
#define BUFFER_HEADS_DAT                (0x4771)    /* RealPt (HEADS.DAT, Intro: ATTIC, FANPRO.NVF, DSALOGO.DAT, GENTIT.DAT) */
#define BUFFER_TEXT                     (0x4775)
#define BUFFER_FONT6                    (0x4779)

#define TEXT_X_END                      (0x478d)     /* Bit16s */

#define GEN_PTR6                        (0x47a3)
#define BUFFER_DMENGE_DAT               (0x47a7)     /* RealPt DMENGE.DAT */

#define GEN_PTR5                        (0x47b3)     /* RealPt */
#define GEN_PTR4                        (0x47b7)     /* RealPt */
#define GEN_PTR3                        (0x47bb)     /* RealPt */
#define GEN_PTR2                        (0x47bf)     /* RealPt */

#define VGA_MEMSTART                    (0x47cb)     /* RealPt */
#define PAGE_BUFFER                     (0x47cf)     /* RealPt */
#define GEN_PTR1_DIS                    (0x47d3)     /* RealPt */
#endif

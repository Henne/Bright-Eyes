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

#define CD_INSERT_COUNTER               (0x00a1)    /* Bit16s = 5 */
#define STR_INSERT_CD                   (0x00a3)    /* char[142]; */
#define STR_REPEAT                      (0x0131)    /* char[12]; */
#define STR_QUIT                        (0x013d)    /* char[8]; */
#define STR_CD_EXEPATH                  (0x0145)    /* char[19]; */

#if 0
#define CD_AUDIO_PAUSED                 (0x00a1)    /* unsigned short; {0,1} */
#define CD_AUDIO_TRACK_BAK              (0x00a3)    /* unsigned short */
#define CD_AUDIO_TRACKLIST              (0x00a5)    /* signed short[15] */
#define CD_CHECK_SKIPMAGIC              (0x00c3)    /* unsigned long */
#define CD_CHECK_ERR_COUNTDOWN          (0x00c7)    /* unsigned short */

#define STR_CD_MISSING                  (0x017e)    /* char[204]; "DIESES PROGRAMM IST OHNE CD-ROM-LAUFWERK NICHT LAUFF\x8eHIG. BITTE INSTALLIEREN SIE DIE GER\x8eTETREIBER F\x9aR DAS CD-ROM-LAUFWERK GEM\x8eSS IHRER CD-ROM-DOKUMENTATION UND STARTEN SIE DIE 'SCHICKSALSKLINGE' ERNEUT." */
#define STR_CD_INIT                     (0x024a)    /* char[37]; "INITIALISIERE CD-ROM IN LAUFWERK %c:" */
#define CD_AUDIO_PAUSE_POS              (0xbc38)    /* unsigned long */
#define CD_AUDIO_PAUSE_TOD              (0xbc3c)    /* unsigned long */
#endif

#define SPELL_TAB                       (0x0158)    /* Bit8s[86][5] */
#define HOUSE_SPELLS                    (0x0387)    /* RealPt[9] */
#define SCHOOL_TAB                      (0x03ab)    /* Bit16s[9][2] */
#define REQ_ATTRIB                      (0x03cf)    /* Bit8u */
#define REQ_REQUIREMENT                 (0x03d0)    /* Bit8u */
#define SKILLS                          (0x0437)    /* Bit8s[13][52] */
#define SPELLS                          (0x06db)    /* Bit8s[6][86] */

#define INIT_LE                         (0x08df)    /* Bit16s[13] */
#define INIT_AE                         (0x08f9)    /* Bit16s[13] */

#define HEIGHT_RANGE                    (0x0913)    /* Bit8u[13]. */
#define WEIGHT_MOD                      (0x092d)    /* Bit8u[13] */
#define MR_MOD                          (0x093a)    /* Bit8s[13] */

#define MONEY_TAB                       (0x0a51)    /* RealPt[13] */
#define INITIAL_SKILL_INCS              (0x0a84)    /* Bit8s[13] */
#define INITIAL_SPELL_INCS              (0x0a91)    /* Bit8s[6] */
#define INITIAL_CONV_INCS               (0x0a97)    /* Bit8s[6] */
#define HOUSE_MOD                       (0x0a9d)    /* struct house_mode[9] */
#define AUTOSKILLS                      (0x0ba2)    /* Bit16s[13][90] */
#define AUTOSPELLS                      (0x0ee0)    /* */

#define HEAD_FIRST_MALE                 (0x1048)    /* Bit8u[12] : start indices in HEADS.DAT for each typus */
#define HEAD_FIRST_FEMALE               (0x1054)    /* Bit8u[12] : start indices in HEADS.DAT for each typus */
#define ATTRIB_COORDS_X                 (0x105f)    /* Bit16s[14] */
#define ATTRIB_COORDS_Y                 (0x1061)    /* Bit16s[14] */

#define MASK_SWITCH                     (0x109a)    /* Bit8s */

#define PAL_TMP                         (0x113b)    /* RGB[32] */
#define PAL_GENBG                       (0x119b)    /* RGB[32] */

#define SCREEN_VAR                      (0x11fe)    /* Bit16s */

#define CHR_LOOKUP                      (0x1b85)
#define MOUSE_MASK                      (0x1200)
#define MOUSE_POSY_MIN                  (0x1240)
#define MOUSE_POSX_MIN                  (0x1242)
#define MOUSE_POSY_MAX                  (0x1244)
#define MOUSE_POSX_MAX                  (0x1246)
#define MOUSE_LOCKED                    (0x1248)
#define MOUSE_REFRESH_FLAG              (0x124a)    /* signed short */
#define MOUSE_POSX                      (0x124c)
#define MOUSE_POSY                      (0x124e)
#define MOUSE_POSX_BAK                  (0x1250)    /* Bit16s */
#define MOUSE_POSY_BAK                  (0x1252)    /* Bit16s */
#define MOUSE_MOVED                     (0x1254)
#define MOUSE_POINTER_OFFSETX           (0x1256)
#define MOUSE_POINTER_OFFSETY           (0x1258)
#define MOUSE_POINTER_OFFSETX_BAK       (0x125a)
#define MOUSE_POINTER_OFFSETY_BAK       (0x125c)


#define DEFAULT_ACTION                  (0x1272)    /* RealPt */
#define ACTION_TABLE                    (0x1276)    /* RealPt */

#define GEN_PAGE                        (0x1324)    /* signed short */

#define USELESS_VARIABLE                (0x1326)    /* Bit8s */
#define TEXT_X_MOD                      (0x1327)    /* Bit16s */
#define RO_ZERO                         (0x1329)    /* Bit16s */

/* struct hero */
#define HERO_NAME                       (0x132c)    /* char[16] */
#define HERO_ALIAS                      (0x133c)    /* char[16] */
#define HERO_TYPUS                      (0x134d)    /* Bit8s */
#define HERO_SEX                        (0x134e)    /* Bit8s */
#define HERO_HEIGHT                     (0x134f)    /* Bit8u */
#define HERO_WEIGHT                     (0x1350)    /* Bit16s */
#define HERO_GOD                        (0x1352)    /* Bit8s */
#define HERO_LEVEL                      (0x1353)    /* Bit8s */
#define HERO_MONEY                      (0x1358)    /* Bit32s */

#define HERO_ATT0_NORMAL                (0x1360)    /* Bit8s */
#define HERO_ATT0_CURRENT               (0x1361)    /* Bit8s */
#define HERO_ATT0_MOD                   (0x1362)    /* Bit8s */

/* Single values used in GEN */
#define HERO_ATT_KL_NORMAL              (0x1363)    /* Bit8s */
#define HERO_ATT_CH_NORMAL              (0x1366)    /* Bit8s */
#define HERO_ATT_CH_CURRENT             (0x1367)    /* Bit8s */
#define HERO_ATT_GE_NORMAL              (0x136c)    /* Bit8s */
#define HERO_ATT_IN_NORMAL              (0x136f)    /* Bit8s */
#define HERO_ATT_KK_NORMAL              (0x1372)    /* Bit8s */
#define HERO_ATT_KK_CURRENT             (0x1373)    /* Bit8s */
#define HERO_ATT_AG_NORMAL              (0x1375)    /* Bit8s */

#define HERO_LE                         (0x138a)    /* Bit16s */
#define HERO_LE_MAX                     (0x138c)    /* Bit16s */
#define HERO_AE                         (0x138e)    /* Bit16s */
#define HERO_AE_MAX                     (0x1390)    /* Bit16s */
#define HERO_MR                         (0x1392)    /* Bit8s */
#define HERO_ATPA_BASE                  (0x1393)    /* Bit8s */
#define HERO_AT_WEAPON                  (0x1394)    /* Bit8s[7] */
#define HERO_PA_WEAPON                  (0x139b)    /* Bit8s[7] */
#define HERO_GROUP                      (0x13b4)    /* Bit8s */

#define HERO_SKILLS                     (0x1434)    /* Bit8s[52] */
#define HERO_SKILL_INCS                 (0x1468)    /* Bit8s */
#define HERO_SPELLS                     (0x1469)    /* Bit8s[86] */
#define HERO_SPELL_INCS                 (0x14bf)    /* Bit8s */
#define HERO_SPELL_SCHOOL               (0x14c0)    /* Bit8s */
#define HERO_STAFF_LEVEL                (0x14c1)    /* Bit8s */

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
#define BOOL_MODE                       (0x1c77)    /* Bit16s */
#define ACTION_PAGE                     (0x1c79)    /* RealPt[11] */

#define NEED_REFRESH                    (0x1ca5)    /* Bit8s {0,1} */
#define TYPE_BITMAP                     (0x1ca6)    /* Bit8[13] */
#define STR_VERSION                     (0x1cb3)    /* char[6] */
#define PAL_ATTIC                       (0x1cb9)    /* RGB[16] */
#define PAL_DSALOGO                     (0x1ce9)    /* RGB[32] */
#define PAL_COL_WHITE                   (0x1d49)    /* RGB[1] */
#define PAL_COL_BLACK                   (0x1d4c)    /* RGB[1] */
#define PAL_POPUP                       (0x1d4f)    /* RGB[8] */
#define PAL_MISC                        (0x1d67)    /* RGB[3] */
#define PAL_HEADS                       (0x1d70)    /* RGB[32] */
#define STR_SOUND_CFG                   (0x1dd0)    /* char[9]; "SOUND.CFG" */
#define STR_SOUND_ADV                   (0x1dda)    /* char[]; "SOUND.ADV */
#define STR_SOUNDHW_NOT_FOUND           (0x1de4)    /* char[]; "SOUND HARDWARE NOT FOUND" */
#define STR_CHR                         (0x1dfe)    /* char[5]; ".CHR" */
#define STR_TEMP_DIR                    (0x1e03)    /* char[] */
#define STR_SAVE_ERROR                  (0x1e09)    /* char[] */

#define STR_DSAGEN_DAT                  (0x1fb0)    /* char[] */
#define STR_MALLOC_ERROR                (0x1fbb)    /* char[] */

#define RANDOM_GEN_SEED                 (0x1fd6)    /* Bit16u */

#define _CTYPE                          (0x1ff9)    /* CLIB array for isalpha() */

#define CD_AUDIO_TRACK                  (0x245a)    /* unsigned short */
#define CD_AUDIO_TOD                    (0x2464)    /* unsigned long */
#define CD_AUDIO_POS                    (0x2468)    /* unsigned long */
#define CD_DRIVE_NO                     (0x246c)    /* unsigned short */

#define CURRENT_TIMBRE_LENGTH           (0x2474)    /* Bit16u */
#define CURRENT_TIMBRE_PATCH            (0x2476)    /* Bit8s */
#define CURRENT_TIMBRE_BANK             (0x2477)    /* Bit8s */
#define CURRENT_TIMBRE_OFFSET           (0x2478)    /* Bit32u */
#define TIMER_ISR_BAK                   (0x247c)    /* RealPt */

#define GOT_CH_BONUS                    (0x2780)    /* Bit16s */
#define GOT_MU_BONUS                    (0x2782)    /* Bit16s */

#define FLEN                            (0x3f2a)    /* Bit32s */
#define FLEN_LEFT                       (0x3f2e)    /* Bit32s */
#define IRQ78_BAK                       (0x3f32)    /* RealPt */
#define GENDAT_OFFSET                   (0x3f36)    /* Bit32s */
#define HANDLE_TIMBRE                   (0x3f3a)    /* Bit16s */
#define TIMBRE_CACHE_SIZE               (0x3f3c)    /* Bit16s */
#define STATE_TABLE_SIZE                (0x3f3e)    /* Bit32u */
#define SND_DRIVER                      (0x3f42)    /* RealPt */
#define FORM_XMID                       (0x3f46)    /* RealPt */
#define SND_TIMBRE_CACHE                (0x3f4a)    /* RealPt */
#define STATE_TABLE                     (0x3f4e)    /* RealPt */
#define SND_DRIVER_BASE_ADDR            (0x3f52)    /* RealPt */
#define SND_DRIVER_DESC                 (0x3f56)    /* RealPt */
#define SND_SEQUENCE                    (0x3f5a)    /* Bit16s */
#define SND_DRIVER_HANDLE               (0x3f5c)    /* Bit16s */
#define PARAM_LEVEL                     (0x3f5e)    /* Bit16s DE: {0, 'a', 'n'} */

#define CALLED_WITH_ARGS                (0x3f60)    /* Bit16s */
#define SPELL_INCS                      (0x3f62)    /* 2 * Bit8s */
#define SKILL_INCS                      (0x400e)    /* 2 * Bit8s */

#define ATTRIB_CHANGED                  (0x4076)    /* Bit8s */
#define TYPE_NAMES                      (0x4084)    /* RealPt[13] */

#define HEAD_LAST                       (0x40b4)    /* Bit8s */
#define HEAD_FIRST                      (0x40b5)    /* Bit8s */
#define HEAD_CURRENT                    (0x40b6)    /* Bit8s */
#define HEAD_TYPUS                      (0x40b7)    /* Bit8s */
#define IN_INTRO                        (0x40b8)    /* Bit8s */
#define MENU_TILES                      (0x40b9)    /* Bit16s */
#define LEFT_BORDER                     (0x40bb)    /* Bit16s */
#define UPPER_BORDER                    (0x40bd)    /* Bit16s */
#define LEVEL                           (0x40bf)    /* Bit16s {-1, 0, 1 (= Novice), 2 (= ADVANCED)}*/
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

#define COL_INDEX                       (0x477d)     /* Bit16s */
#define BG_COLOR                        (0x477f)     /* Bit16s */
#define FG_COLOR                        (0x4781)     /* Bit16s[6] */
#define TEXT_X_END                      (0x478d)     /* Bit16s */
#define TEXT_Y                          (0x478f)     /* Bit16s */
#define TEXT_X                          (0x4791)     /* Bit16s */

#define PICBUF3                         (0x4797)     /* RealPt */
#define PICBUF2                         (0x479b)     /* RealPt */
#define PICBUF1                         (0x479f)     /* RealPt */
#define GEN_PTR6                        (0x47a3)
#define BUFFER_DMENGE_DAT               (0x47a7)     /* RealPt DMENGE.DAT */

#define GEN_PTR5                        (0x47b3)     /* RealPt */
#define GEN_PTR4                        (0x47b7)     /* RealPt */
#define GEN_PTR3                        (0x47bb)     /* RealPt */
#define GEN_PTR2                        (0x47bf)     /* RealPt */

#define GFX_PTR                         (0x47c7)     /* RealPt */
#define VGA_MEMSTART                    (0x47cb)     /* RealPt */
#define PAGE_BUFFER                     (0x47cf)     /* RealPt */
#define GEN_PTR1_DIS                    (0x47d3)     /* RealPt */
#define WO_VAR2                         (0x47d7)     /* Bit16s */
#define WO_VAR3                         (0x47d9)     /* Bit16s */
#define DISPLAY_PAGE_BAK                (0x47db)     /* Bit16s */
#define DISPLAY_MODE_BAK                (0x47dd)     /* Bit16s */
#define RO_VAR                          (0x47e3)     /* RealPt */
#endif

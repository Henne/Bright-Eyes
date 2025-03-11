/*
 *	symbol names for version v3.02_de
 *
 *	Here are the mappings from adresses in the datasegment
 *	to symbolic names of global variables.
 *	This will only work with the version v302de.
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

#define MIDI_DISABLED			(0x1a07)    /* signed short */
#define USE_CDA				(0x1a09)    /* signed short */

#define STR_SOUND_CFG			(0x1dd0)    /* char[9]; "SOUND.CFG" */

#define RANDOM_GEN_SEED			(0x1fd6)    /* Bit16u */

#define CD_AUDIO_TRACK                  (0x245a)    /* unsigned short */
#define CD_AUDIO_TOD                    (0x2464)    /* unsigned long */
#define CD_AUDIO_POS                    (0x2468)    /* unsigned long */
#define CD_DRIVE_NO                     (0x246c)    /* unsigned short */

#define HANDLE_TIMBRE			(0x3f3a)    /* Bit16s */
#define SND_DRIVER			(0x3f42)    /* RealPt */
#define FORM_XMID			(0x3f46)    /* RealPt */
#define SND_PTR_UNKN1			(0x3f4a)    /* RealPt */
#define STATE_TABLE			(0x3f4e)    /* RealPt */

#define SND_SEQUENCE			(0x3f5a)    /* Bit16s */
#define SND_DRIVER_HANDLE		(0x3f5c)    /* Bit16s */

#define RANDOM_GEN_SEED2		(0x458f)    /* Bit16u */

#endif

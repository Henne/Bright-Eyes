#if !defined(__BORLANDC__)
namespace M302de {
#endif

//static
unsigned short CD_set_drive_nr(void);
//static
void CD_driver_request(RealPt req);
//static
Bit32s CD_get_tod(void);
//static
void seg001_00c1(unsigned short);
void seg001_02c4(void);
signed short CD_bioskey(signed short);
//static
void CD_audio_stop_hsg(void);
void CD_audio_stop(void);
void CD_audio_pause(void);
void CD_audio_play(void);
void CD_set_track(signed short);
void CD_check(void);
void CD_init(void);

#if !defined(__BORLANDC__)
}
#endif

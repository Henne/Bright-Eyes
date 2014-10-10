#if !defined(__BORLANDC__)
namespace M302de {
#endif

//static
unsigned short CD_set_drive_nr();
//static
void CD_driver_request(RealPt req);
//static
unsigned int CD_get_tod();
//static
void seg001_00c1(unsigned short);
void seg001_02c4(void);
signed short CD_bioskey(signed short);
//static
void CD_audio_stop_hsg(void);
void CD_audio_stop();
void CD_audio_pause();
void CD_audio_play();
void CD_set_track(signed short);
void CD_check(void);
void CD_init(void);

#if !defined(__BORLANDC__)
}
#endif

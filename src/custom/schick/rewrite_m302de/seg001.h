namespace M302de {

//static
unsigned short CD_set_drive_nr();
//static
void CD_driver_request(RealPt req);
//static
unsigned int CD_get_tod();
//static
void seg001_00c1(unsigned short);
void seg001_02c4();
signed short CD_bioskey(signed short);
//static
void seg001_0322();
//static
void seg001_034f();
void CD_audio_pause();
void CD_audio_play();

}

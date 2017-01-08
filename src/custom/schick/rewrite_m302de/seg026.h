#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
void init_text(void);
//0x25
void split_textbuffer(Bit8u *, RealPt, Bit32u);
//0x2a
void load_ggsts_nvf(void);
/* 0x2f */
signed short load_game_state(void);
/* 0x34 */
signed short save_game_state(void);
/* 0x39 */
signed short read_chr_temp(RealPt, signed short, signed short);
//0x3e
void write_chr_temp(unsigned short);
/* 0x43 */
signed short copy_chr_names(Bit8u*, signed short);
//0x48
void load_in_head(signed short);
//0x4d
void load_tx2(signed short);
//0x57
void load_tx(signed short);
//0x57
void load_ltx(unsigned short);
//0x61
void prepare_chr_name(char*, char*);
//0x61
void prepare_sg_name(char*, char*);
//0x66
void load_tempicon(signed short);

#if !defined(__BORLANDC__)
}
#endif

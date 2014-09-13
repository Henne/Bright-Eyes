#if !defined(__BORLANDC__)
namespace M302de {
#endif

//static
unsigned short FIG_obj_needs_refresh(Bit8u*, signed short, signed short);
//static
void FIG_set_star_color(Bit8u*, unsigned short, unsigned char);
//static
RealPt FIG_name_3rd_case(unsigned short, volatile unsigned short);
//static
RealPt FIG_name_4th_case(unsigned short, volatile unsigned short);
//static
RealPt FIG_name_1st_case(unsigned short, volatile unsigned short);
//static
unsigned short fight_printer(void);

void draw_fight_screen(Bit16u);
//static
void set_delay_timer(void);
//static
void fight_delay(void);
#if !defined(__BORLANDC__)
}
#endif

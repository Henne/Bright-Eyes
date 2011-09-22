namespace M302de {

//static
unsigned short FIG_obj_needs_refresh(Bit8u *p, signed short x, signed short y);
//static
void FIG_set_star_color(PhysPt ptr, unsigned short count, unsigned char val);
//static
RealPt FIG_name_3rd_case(unsigned short type, unsigned short pos);
//static
RealPt FIG_name_4th_case(unsigned short type, unsigned short pos);
//static
RealPt FIG_name_1st_case(unsigned short type, unsigned short pos);

void seg005_0598(Bit16u);
//static
void set_delay_timer();
//static
void fight_delay();
}

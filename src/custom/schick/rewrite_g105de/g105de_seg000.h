namespace G105de {
RealPt bc__dos_getvect(Bit16s);
void bc__dos_setvect(Bit16s, RealPt);
Bit32s bc_lseek(Bit16u, Bit32u, Bit16s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
signed short bioskey(signed short);
Bit16s bc_close(Bit16u);
Bit16s bc__close(Bit16u);
Bit16s bc__create(Bit8u*, Bit16u);
Bit16s bc_write(Bit16u, Bit8u*, Bit16u);

}

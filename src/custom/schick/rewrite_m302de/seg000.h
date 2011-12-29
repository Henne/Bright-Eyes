namespace M302de {

Bit32s bc_lseek(Bit16u, Bit32u, Bit16s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
signed short bioskey(signed short);
RealPt bc_farcalloc(Bit32u, Bit32u);
Bit16s bc_close(Bit16u);
Bit16s bc__close(Bit16u);
RealPt bc_memmove(RealPt, RealPt, Bit16u);
Bit16s bc__creat(RealPt, Bit16u);

Bit32s bc__write(Bit16u, RealPt, Bit16u);

}

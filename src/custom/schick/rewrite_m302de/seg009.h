#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if !defined(__BORLANDC__)
void decomp_pp20(Bit8u *dst, Bit8u *src_data, Bit8u *src, Bit32u len);
#else
void decomp_pp20(Bit8u *dst, Bit8u *src_data, Bit16u off, Bit16u seg, Bit32u len);
#endif

#if !defined(__BORLANDC__)
}
#endif

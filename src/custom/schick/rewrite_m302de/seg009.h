#if !defined(__BORLANDC__)
namespace M302de {

void decomp_pp20(Bit8u *dst, Bit8u *src_data, Bit8u *src, Bit32u len);

}

#else
extern "C" {

void decomp_pp20(Bit8u *dst, Bit8u *src_data, Bit16u off, Bit16u seg, Bit32u len);

}
#endif

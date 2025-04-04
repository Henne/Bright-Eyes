#ifndef PORT_H
#define PORT_H

#if defined(__BORLANDC__)

#ifdef __cplusplus
#define INTCAST void interrupt (*)(...)
#else
#define INTCAST void interrupt (*)()
#endif

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;

typedef Bit8u* RealPt;
typedef Bit8u* PhysPt;
typedef Bit8u huge * HugePt;

#define RealMake(seg, off) ((RealPt)(MK_FP(seg, off)))
#define Real2Phys(p) (p)
#define Real2Host(p) ((Bit8u*)(p))


#define F_PADA(p, o) (*((HugePt*)p) += o)
#define F_PADD(p, o) ((HugePt)(p) + o)
#define F_PSUB(p1, p2) ((HugePt)(p1) - (HugePt)(p2))

#define H_PADD(p, o) F_PADD(p, o)

extern char ds[0x488f];
//#define p_datseg ((Bit8u*)(&ds[0x0000]))
#define datseg (_DS)
//#define ds (RealMake(datseg, 0x0000))

#define main_gen main
#define reloc_gen (0)


#define ds_readb(p) *(unsigned char*)((ds) + p)
#define ds_readw(p) *(unsigned short*)((ds) + p)
#define ds_readd(p) *(unsigned long*)((ds) + p)

#define ds_readbs(p) *(signed char*)((ds) + p)
#define ds_readws(p) *(signed short*)((ds) + p)
#define ds_readds(p) *(signed long*)((ds) + p)

#define ds_writeb(p, d) (*(unsigned char*)((ds) + (p)) = (d))
#define ds_writew(p, d) (*(unsigned short*)((ds) +(p)) = (d))
#define ds_writed(p, d) *(unsigned long*)((ds) + (p)) = (d)

#define ds_writebs(p, d) (*(signed char*)((ds) + (p)) = (d))
#define ds_writews(p, d) *(signed short*)((ds) + (p)) = (d)
#define ds_writeds(p, d) (*(signed long*)((ds) + (p)) = (d))

#define ds_inc_b(p)	(++*(unsigned char*)((ds) + (p)))
#define ds_inc_w(p)	(++*(unsigned short*)((ds) + (p)))
#define ds_inc_d(p)	(++*(unsigned long*)((ds) + (p)))

#define ds_dec_b(p)	(--*(unsigned char*)((ds) + (p)))
#define ds_dec_w(p)	(--*(unsigned short*)((ds) + (p)))
#define ds_dec_d(p)	(--*(unsigned long*)((ds) + (p)))

#define ds_inc_bs(p)	(++*(signed char*)((ds) + (p)))
#define ds_inc_ws(p)	(++*(signed short*)((ds) + (p)))
#define ds_inc_ds(p)	(++*(signed long*)((ds) + (p)))

#define ds_dec_bs(p)	(--*((signed char*)((ds) + (p))))
#define ds_dec_ws(p)	(--*((signed short*)((ds) + (p))))
#define ds_dec_ds(p)	(--*((signed long*)((ds) + (p))))

#define ds_and_bs(p, d)	((*(signed char*)((ds) + p)) &= (d))
#define ds_add_bs(p, d)	((*(signed char*)((ds) + p)) += (d))
#define ds_sub_bs(p, d)	((*(signed char*)((ds) + p)) -= (d))

#define ds_sub_ds(p, d)	((*(signed long*)((ds) + p)) -= (d))

#define ds_and_ws(p, d)	((*(signed short*)((ds) + p)) &= (d))
#define ds_add_ws(p, d)	((*(signed short*)((ds) + p)) += (d))
#define ds_xor_bs(p, d)	((*(signed char*)((ds) + p)) ^= (d))

#define ds_inc_bs_post(p)	(*(signed char*)((ds) + (p)))++
#define ds_dec_bs_post(p)	(*(signed char*)((ds) + (p)))--


#define mem_readb(p) *(signed char*)(p)
#define mem_readw(p) *(signed short*)(p)
#define mem_readd(p) *(signed long*)(p)

#define mem_writeb(p, d) *(unsigned char*)(p) = d
#define mem_writew(p, d) *(unsigned short*)(p) = d
#define mem_writed(p, d) *(unsigned long*)(p) = d

#define host_readb(p) (*(Bit8u*)(p))
#define host_readw(p) (*(Bit16u*)(p))
#define host_readd(p) (*(Bit32u*)(p))

#define host_readbs(p) *(Bit8s*)(p)
#define host_readws(p) *(Bit16s*)(p)
#define host_readds(p) *(Bit32s*)(p)

#define host_writeb(p, d)       (*(Bit8u*)(p) = d)
#define host_writew(p, d)       (*(Bit16u*)(p) = d)
#define host_writed(p, d)       (*(Bit32u*)(p) = d)

#define host_writebs(p, d)       (*(Bit8s*)(p) = d)
#define host_writews(p, d)       (*(Bit16s*)(p) = d)
#define host_writeds(p, d)       (*(Bit32s*)(p) = d)

#define host_inc_bs(p)		((*(Bit8s*)(p))++)
#define host_dec_bs(p)		((*(Bit8s*)(p))--)

#else
#define INTCAST RealPt

#define __abs__(v) abs(v)

static inline Bit8s host_readbs(Bit8u* p)
{
	return (Bit8s)host_readb(p);
}

static inline Bit16s host_readws(Bit8u* p)
{
	return (Bit16s)host_readw(p);
}

static inline Bit32s host_readds(Bit8u* p)
{
	return (Bit32s)host_readd(p);
}

static inline Bit8s host_writebs(Bit8u* p, Bit8s val)
{
	return (*(Bit8s*)(p) = val);
}

static inline Bit16s host_writews(Bit8u* p, Bit16s val)
{
	return (*(Bit16s*)(p) = val);
}

static inline Bit8s host_inc_bs(Bit8u* p)
{
	return ++(*(Bit8s*)(p));
}

static inline Bit8s host_dec_bs(Bit8u* p)
{
	return --(*(Bit8s*)(p));
}

static inline Bit16s ds_add_bs(Bit16s off, Bit8s v)
{
	return *(Bit8s*)(p_datseg + off) += v;
}

static inline Bit16s ds_sub_bs(Bit16s off, Bit8s v)
{
	return *(Bit8s*)(p_datseg + off) -= v;
}

static inline Bit32s ds_sub_ds(Bit16s off, Bit16u v)
{
	return *(Bit32s*)(p_datseg + off) -= v;
}

static inline Bit16s ds_add_ws(Bit16s off, Bit16s v)
{
	return *(Bit16s*)(p_datseg + off) += v;
}

static inline Bit8s ds_xor_bs(Bit16s off, Bit8s v)
{
	return *(Bit8s*)(p_datseg + off) ^= v;
}

static inline Bit8s ds_inc_bs_post(Bit16u off)
{
	Bit8s v = ds_readbs(off);
	ds_writebs(off, v + 1);
	return v;
}

static inline Bit8s ds_dec_bs_post(Bit16u off)
{
	Bit8s v = ds_readbs(off);
	ds_writebs(off, v - 1);
	return v;
}

#endif

#endif

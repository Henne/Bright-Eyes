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

extern char ds[0xffff];
#define p_datseg (&ds[0x0000])
#define datseg (_DS)

#define ds_readb(p) *(unsigned char*)(ds + p)
#define ds_readw(p) *(unsigned short*)(ds + p)
#define ds_readd(p) *(unsigned long*)(ds + p)

#define ds_readbs(p) *(signed char*)(ds + p)
#define ds_readws(p) *(signed short*)(ds + p)
#define ds_readds(p) *(signed long*)(ds + p)

#define ds_writeb(p, d) *(unsigned char*)(ds + p) = d
#define ds_writew(p, d) *(unsigned short*)(ds + p) = d
#define ds_writed(p, d) *(unsigned long*)(ds + p) = d

#define ds_inc_b(p)	++*(unsigned char*)(ds + p)
#define ds_inc_w(p)	++*(unsigned short*)(ds + p)
#define ds_inc_d(p)	++*(unsigned long*)(ds + p)

#define ds_dec_b(p)	--*(unsigned char*)(ds + p)
#define ds_dec_w(p)	--*(unsigned short*)(ds + p)
#define ds_dec_d(p)	--*(unsigned long*)(ds + p)

#define ds_inc_bs(p)	++*(signed char*)(ds + p)
#define ds_inc_ws(p)	++*(signed short*)(ds + p)
#define ds_inc_ds(p)	++*(signed long*)(ds + p)

#define ds_dec_bs(p)	--*((signed char*)(ds + p))
#define ds_dec_ws(p)	--*((signed short*)(ds + p))
#define ds_dec_ds(p)	--*((signed long*)(ds + p))

#define ds_and_ws(p, d)	((*(signed short*)(ds + p)) &= d)

#define mem_readb(p) *(signed char*)(p)
#define mem_readw(p) *(signed short*)(p)
#define mem_readd(p) *(signed long*)(p)

#define mem_writeb(p, d) *(unsigned char*)(p) = d
#define mem_writew(p, d) *(unsigned short*)(p) = d
#define mem_writed(p, d) *(unsigned long*)(p) = d

#define host_readb(p) *(Bit8u*)(p)
#define host_readw(p) *(Bit16u*)(p)
#define host_readd(p) *(Bit32u*)(p)

#define host_readbs(p) *(Bit8s*)(p)
#define host_readws(p) *(Bit16s*)(p)
#define host_readds(p) *(Bit32s*)(p)

#define host_writeb(p, d)       (*(Bit8u*)(p) = d)
#define host_writew(p, d)       (*(Bit16u*)(p) = d)
#define host_writed(p, d)       (*(Bit32u*)(p) = d)

#define host_writebs(p, d)       (*(Bit8s*)(p) = d)
#define host_writews(p, d)       (*(Bit16s*)(p) = d)
#define host_writeds(p, d)       (*(Bit32s*)(p) = d)

#define RealMake(seg, off) ((RealPt)((seg << 4) + off))
#define Real2Phys(p) (p)
#define Real2Host(p) (p)

#else
#define INTCAST RealPt

static inline Bit8s host_readbs(Bit8u* p)
{
	return (Bit8s)host_readb(p);
}
#endif

#endif

#ifndef PORT_H
#define PORT_H

#if defined(__BORLANDC__)

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;

typedef Bit8u* RealPt;
typedef Bit8u* PhysPt;

typedef int bool;


extern char ds[0xffff];
#define p_datseg (&ds[0x0000])

#define ds_readb(p) *(signed char*)(ds + p)
#define ds_readw(p) *(signed short*)(ds + p)
#define ds_readd(p) *(signed long*)(ds + p)

#define ds_writeb(p, d) *(unsigned char*)(ds + p) = d
#define ds_writew(p, d) *(unsigned short*)(ds + p) = d
#define ds_writed(p, d) *(unsigned long*)(ds + p) = d

#define mem_readb(p) *(signed char*)(p)
#define mem_readw(p) *(signed short*)(p)
#define mem_readd(p) *(signed long*)(p)

#define mem_writeb(p, d) *(unsigned char*)(p) = d
#define mem_writew(p, d) *(unsigned short*)(p) = d
#define mem_writed(p, d) *(unsigned long*)(p) = d

#define host_readb(p) *(Bit8u*)(p)
#define host_readw(p) *(Bit16u*)(p)
#define host_readd(p) *(Bit32u*)(p)

#define host_writeb(p, d)       (*(Bit8u*)(p) = d)
#define host_writew(p, d)       (*(Bit16u*) = d)
#define host_writed(p, d)       (*(Bit32u*)(p) = d)

#define RealMake(seg, off) (void*)(seg << 4 + off)
#define Real2Host(p) (p)

#define false (0)
#define true (1)

#define bc_free(p) free(p)
#define emu_gen_alloc(nelem) calloc(1, nelem)

#endif

#endif

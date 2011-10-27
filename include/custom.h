#ifndef DOSBOX_CUSTOM_H
#define DOSBOX_CUSTOM_H

#include "dosbox.h"

class custom_prog {
	public:
		virtual bool probe(char *name, Bit16u relocate,
				Bit16u init_cs, Bit16u init_ip) = 0;
		virtual void remove(Bit8u exitcode) = 0;
		virtual void suspend() = 0;
		virtual void resume() = 0;

		virtual int fcall_handler(Bitu seg, Bitu off) = 0;
		virtual int ncall_handler(Bit16u off) = 0;
	private:
};

#if DOSBOX_CUSTOM

void custom_init(Section *sec);

/* prototypes for Execution operations */
void custom_init_prog(char *, Bit16u, Bit16u, Bit16u);
void custom_exit_prog(Bit8u);

/* prototypes for CPU operations */
int custom_callf(Bitu, Bitu);
int custom_calln(Bit16u);

#else /* DOSBOX_CUSTOM */

static inline void custom_init(Section *sec) { }

/* prototypes for Execution operations */
static inline void
custom_init_prog(char *name, Bit16u relocate, Bit16u init_cs, Bit16u init_ip) { }

static inline void custom_exit_prog(Bit8u exitcode) { }

/* prototypes for CPU operations */
static inline int custom_callf(Bitu seg, Bitu off) { return 0; }
static inline int custom_calln(Bit16u off) { return 0; }

#endif /* DOSBOX_CUSTOM */

#endif /* DOSBOX_CUSTOM_H */

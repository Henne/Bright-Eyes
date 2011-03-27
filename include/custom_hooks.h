#ifndef DOSBOX_SCHICK_H
#define DOSBOX_SCHICK_H

#ifdef DOSBOX_CUSTOM

/* prototypes for Execution operations */
void init_schick(char *, unsigned short, unsigned short, unsigned short);
void exit_schick(unsigned char);

/* prototypes for CPU operations */
int schick_callf(unsigned, unsigned);
int schick_calln16(unsigned);

#else /* DOSBOX_CUSTOM */

/* prototypes for Execution operations */
inline void init_schick(char *, unsigned short, unsigned short, unsigned short) {};
inline void exit_schick(unsigned char) {};

/* prototypes for CPU operations */
inline int schick_callf(unsigned, unsigned) {return 0;};
inline int schick_calln16(unsigned) {return 0;};

#endif /* DOSBOX_CUSTOM */

#endif /* DOSBOX_SCHICK_H */

/* Make sure that only one part of the three is defined.
   Otherwise, interception of calls will not work as intended. */
#undef DOSBOX_SCHICK_H
#define DOSBOX_SCHWEIF_H
#undef DOSBOX_RIVA_H

#ifdef DOSBOX_CUSTOM

#ifdef  DOSBOX_SCHICK_H
/* prototypes for Execution operations */
bool schick_init(char *, unsigned short, unsigned short, unsigned short);
void schick_exit(unsigned char);

/* prototypes for CPU operations */
int schick_callf(unsigned, unsigned);
int schick_calln16(unsigned);
#endif  /* DOSBOX_SCHICK_H */

#ifdef  DOSBOX_SCHWEIF_H
/* prototypes for Execution operations */
bool schweif_init(char *, unsigned short, unsigned short, unsigned short);
void schweif_exit(unsigned char);

/* prototypes for CPU operations */
int schweif_callf(unsigned, unsigned);
int schweif_calln(unsigned);
#endif  /* DOSBOX_SCHWEIF_H */

#ifdef  DOSBOX_RIVA_H
/* prototypes for Execution operations */
bool riva_init(char *, unsigned short, unsigned short, unsigned short);
void riva_exit(unsigned char);

/* prototypes for CPU operations */
int riva_callf(unsigned, unsigned);
int riva_calln(unsigned);
#endif  /* DOSBOX_RIVA_H */



#else /* DOSBOX_CUSTOM */



#ifdef DOSBOX_SCHICK_H
/* prototypes for Execution operations */
inline bool schick_init(char *, unsigned short, unsigned short, unsigned short) { return false; };
inline void schick_exit(unsigned char) {};

/* prototypes for CPU operations */
inline int schick_callf(unsigned, unsigned) {return 0;};
inline int schick_calln16(unsigned) {return 0;};
#endif /* DOSBOX_SCHICK_H */

#ifdef DOSBOX_SCHWEIF_H
inline bool schweif_init(char *, unsigned short, unsigned short, unsigned short) { return false; };
inline void schweif_exit(unsigned char) {};

/* prototypes for CPU operations */
inline int schweif_callf(unsigned, unsigned) {return 0;};
inline int schweif_calln(unsigned) {return 0;};
#endif /* DOSBOX_SCHWEIF_H */

#ifdef DOSBOX_RIVA_H
inline bool riva_init(char *, unsigned short, unsigned short, unsigned short) { return false; };
inline void riva_exit(unsigned char) {};

/* prototypes for CPU operations */
inline int riva_callf(unsigned, unsigned) {return 0;};
inline int riva_calln(unsigned) {return 0;};
#endif /* DOSBOX_RIVA_H */

#endif /* DOSBOX_CUSTOM */

#define DOSBOX_SOA_H
#define DOSBOX_SCHICK_H
#define DOSBOX_SCHWEIF_H

#ifdef DOSBOX_CUSTOM

bool check_bcc(char *, unsigned short , unsigned short, unsigned short);

Bit32s DB_get_conv_mem();

#ifdef DOSBOX_SOA_H
/* prototypes for Execution operations */
bool soa_init(char *, unsigned short, unsigned short, unsigned short);
void soa_exit(unsigned char);

/* prototypes for CPU operations */
int soa_callf(unsigned, unsigned);
int soa_calln(unsigned);
#else /* DOSBOX_SOA_H */
/* prototypes for Execution operations */
inline bool soa_init(char *, unsigned short, unsigned short, unsigned short) { return false; };
inline void soa_exit(unsigned char) {};

/* prototypes for CPU operations */
inline int soa_callf(unsigned, unsigned) {return 0;};
inline int soa_calln(unsigned) {return 0;};
#endif /* DOSBOX_SOA_H */

#ifdef DOSBOX_SCHICK_H
/* prototypes for Execution operations */
bool init_schick(char *, unsigned short, unsigned short, unsigned short);
void exit_schick(unsigned char);

/* prototypes for CPU operations */
int schick_callf(unsigned, unsigned);
int schick_calln16(unsigned);
#else /* DOSBOX_SCHICK_H */
/* prototypes for Execution operations */
inline bool init_schick(char *, unsigned short, unsigned short, unsigned short) { return false; };
inline void exit_schick(unsigned char) {};

/* prototypes for CPU operations */
inline int schick_callf(unsigned, unsigned) {return 0;};
inline int schick_calln16(unsigned) {return 0;};
#endif /* DOSBOX_SCHICK_H */


#ifdef  DOSBOX_SCHWEIF_H
/* prototypes for Execution operations */
bool schweif_init(char *, unsigned short, unsigned short, unsigned short);
void schweif_exit(unsigned char);

/* prototypes for CPU operations */
int schweif_callf(unsigned, unsigned);
int schweif_calln(unsigned);
#else  /* DOSBOX_SCHWEIF_H */
static inline bool schweif_init(char *, unsigned short, unsigned short, unsigned short) { return false; }
static inline void schweif_exit(unsigned char) {}
static inline int schweif_callf(unsigned, unsigned) {return 0;}
static inline int schweif_calln(unsigned) {return 0;}
#endif  /* DOSBOX_SCHWEIF_H */

#endif /* DOSBOX_CUSTOM */

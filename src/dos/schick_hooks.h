extern void init_schick(char *, unsigned short, unsigned short, unsigned short);
extern void exit_schick(unsigned char);

extern void schick_create(const char *, unsigned char, unsigned);
extern void schick_open(const char *, unsigned char, unsigned);
extern void schick_close(unsigned);

extern void schick_read(unsigned, unsigned char*, unsigned short);
extern void schick_write(unsigned, unsigned char*, unsigned short);

extern void schick_seek(unsigned, unsigned, unsigned);
extern int schick_callf(unsigned, unsigned, unsigned);
extern void schick_ret();
extern void schick_calln16(unsigned);

/* prototypes for Execution operations */
void init_schick(char *, unsigned short, unsigned short, unsigned short);
void exit_schick(unsigned char);

/* prototypes for FILE operations */

void schick_create(const char *, unsigned char, unsigned);
void schick_open(const char *, unsigned char, unsigned);
void schick_close(unsigned);

void schick_read(unsigned, unsigned char*, unsigned short);
void schick_write(unsigned, unsigned char*, unsigned short);

void schick_seek(unsigned, unsigned, unsigned);


/* prototypes for CPU operations */
int schick_callf(unsigned, unsigned);
int schick_calln16(unsigned);
void schick_ret();

struct driver_request {
	char dummy1;		/* */
	char subunit;
	char function;		/* */
	unsigned short status;
	char dummy2[8];
	char redbook;		/* 0 or 1 */
	void *ptr;		/* */
	unsigned short dummy4;		/* 0,1,7,b */
	unsigned short dummy6;		/* 0 or 2 */
	char dummy7[6];
};

extern struct driver_request far req[9];
extern unsigned char far cd_buf1[408];
extern unsigned char far cd_buf2[20];

#if !defined(__BORLANDC__)
namespace G105de {
#endif

	//static
	unsigned short CD_set_drive_nr();

	//static
	void CD_driver_request(RealPt req);
	//static
	unsigned int CD_get_tod();
	//static
	void seg001_00bb(unsigned short);
	//static
	signed short CD_bioskey(signed short);
	//static
	void seg001_0312();

	void seg001_033b();

	void seg001_03a8();

	void seg001_0465();

	bool seg001_0600();
#if !defined(__BORLANDC__)
}
#endif

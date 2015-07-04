#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_merchant(void);

/* 0x2a */
void add_item_to_shop(Bit8u*, signed short, signed short);
/* 0x2f */
void TLK_ghandel(signed short);
/* 0x34 */
void TLK_khandel(signed short);
/* 0x39 */
void TLK_whandel(signed short);

#if !defined(__BORLANDC__)
}
#endif

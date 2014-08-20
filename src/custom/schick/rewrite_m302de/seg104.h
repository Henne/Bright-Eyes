#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short do_alchemy(Bit8u*, signed short, signed short);

/* 0x25 */
/* static */
signed short hero_has_ingrendients(Bit8u*, signed short);

/* 0x2a */
/* static */
void hero_use_ingrendients(Bit8u*, signed short);

/* 0x2f */
signed short plan_alchemy(Bit8u*);

/* 0x34 */
signed short has_herb_for_disease(Bit8u*, signed short);

/* 0x39 */
signed short talent_cure_disease(Bit8u*, Bit8u*, signed short, signed short);

/* 0x3e */
RealPt get_heaviest_hero(void);

/* 0x43 */
signed short get_hero_weight(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif

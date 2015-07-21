#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void delete_chest_item(Bit8u*, signed short);

/* 0x25 */
void chest_protected_normal(void);

/* 0x2a */
void chest_closed(void);

/* 0x2f */
void chest_protected_heavy(void);

/* 0x34 */
void chest_cursed(void);

/* 0x39 */
void chest_fulminictus(void);

/* 0x3e */
void loot_simple_chest(Bit8u*);

/* 0x43 */
void loot_chest(Bit8u*, Bit8u*, Bit8u*);

/* 0x48 */
void seg092_06b4(signed short);

/* 0x4d */
void use_lockpicks_on_chest(RealPt);

/* 0x5c */
void chest_poisoned1(void);

/* 0x61 */
void chest_poisoned2(void);

/* 0x66 */
void chest_poisoned3(void);

/* 0x6b */
void chest_protected_brutal(void);

/* 0x70 */
void chest_stoned(void);

/* 0x75 */
void chest_ignifax_normal(void);

/* 0x7a */
void chest_ignifax_brutal(void);

/* 0x7a */
void chest_ignifax_heavy(void);

/* 0x82 */
void chest_crossbow_bolts(void);

/* 0x89 */
signed short hero_has_lockpicks(Bit8u*);

#if !defined(__BORLANDC__)
}
#endif

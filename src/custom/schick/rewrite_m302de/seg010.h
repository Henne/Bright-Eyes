#if !defined(__BORLANDC__)
namespace M302de {
#endif

unsigned short EMS_get_num_pages_unalloced();
unsigned short EMS_alloc_pages(unsigned short);
unsigned short EMS_free_pages(unsigned short);
unsigned short EMS_map_memory(unsigned short, unsigned short, unsigned char);
RealPt EMS_norm_ptr(RealPt);
unsigned short EMS_init();

#if !defined(__BORLANDC__)
}
#endif

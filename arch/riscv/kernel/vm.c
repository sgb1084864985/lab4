#include"vm.h"
#include"put.h"
  
extern unsigned long long text_start;
extern unsigned long long rodata_start;
extern unsigned long long data_start;
extern unsigned long long _end;


void create_mapping(uint64_t *pgtbl, uint64_t va, uint64_t pa, uint64_t sz, int perm)
{
	/*your code*/
    // close_virtual_mapping();

    static int last_allocated_page_addr=null; // root page
    if(last_allocated_page_addr==null) last_allocated_page_addr=pgtbl;
    root_page = pgtbl

    second_level_page_addr = root_page[va.VPN[2]].PPN
    if second_level_page_addr == null :
        last_allocated_page_addr += 0x1000
        root_page[va.VPN[2]].init()
        root_page[va.VPN[2]].PPN = last_allocated_page_addr
        second_level_page_addr = last_allocated_page_addr
        second_level_page_addr.clear()

    third_level_page_addr = second_level_page_addr[va.VPN[1]].PPN
    if third_level_page_addr == null :
        last_allocated_page_addr += 0x1000
        second_level_page_addr[va.VPN[1]].init()
        second_level_page_addr[va.VPN[1]].PPN = last_allocated_page_addr
        third_level_page_addr = last_allocated_page_addr
        third_level_page_addr.clear()

    third_level_page_addr[va.VPN[0]].init()
    third_level_page_addr[va.VPN[0]].PPN = pa
    third_level_page_addr[va.VPN[0]].RSW = perm
    
    // open_virtual_mapping();
}

void paging_init()
{
    uint64_t *pgtbl = &_end;
    /*your code*/
    const uint64_t BYTE_OFFSET=0xffffffe000000000-0x800000000;
    const uint64_t PAGE_OFFSET=BYTE_OFFSET>>3;
    const uint64_t physical_text_start_addr=(uint64_t)(&text_start);
    const uint64_t physical_rodata_start_addr=(uint64_t)(&rodata_start);
    const uint64_t physical_end_addr=(uint64_t)(&_end);
    const uint64_t physical_text_start_page=physical_text_start_addr>>3;
    const uint64_t physical_rodata_start_page=physical_rodata_start_addr>>3;
    const uint64_t physical_end_page=physical_end_addr>>3;

    /* virtual page number from ffffffe000000 to fffffffffffff, total 16M virtual page number*/
    for(uint64_t pn = 0xffffffe000002;pn<0xfffffffffffff;pn++){
        create_mapping(pgtbl,pn,pn-PAGE_OFFSET,0x1000,6);
    }

    // identical-mapping
    create_mapping(pgtbl,physical_text_start_page,physical_text_start_page,(uint64_t)(0x1000),5);
    create_mapping(pgtbl,physical_rodata_start_page,physical_rodata_start_page,(uint64_t)(0x1000),4);
    create_mapping(pgtbl,physical_text_start_page+PAGE_OFFSET,physical_text_start_page,(uint64_t)(0x1000),5);
    create_mapping(pgtbl,physical_rodata_start_page+PAGE_OFFSET,physical_rodata_start_page,(uint64_t)(0x1000),4);

    for(uint64_t pn = physical_rodata_start_page+1;pn<physical_end_page;pn++){
        create_mapping(pgtbl,pn,pn,0x1000,6);
    }

    // hardware
    for(uint64_t pn = 0x10000;pn<0x80000;pn++){
        create_mapping(pgtbl,pn,pn,0x1000,4);
    }
}

#include <gdt.h>

#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

#define GDT_CODE_PL0       SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | SEG_LONG(1) | SEG_SIZE(0) | SEG_GRAN(0) | SEG_PRIV(0) | SEG_CODE_EXRD
#define GDT_DATA_PL0       SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | SEG_LONG(1) | SEG_SIZE(0) | SEG_GRAN(0) | SEG_PRIV(0) | SEG_DATA_RDWR

extern void gdt_flush(uint64_t base, uint16_t limit);

struct gdt_entry_struct gdt_entries[3];
struct gdt_ptr_struct gdt_ptr;

uint64_t createDescriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    descriptor  =  limit       & 0x000F0000;  
    descriptor |= (flag <<  8) & 0x00F0FF00;    
    descriptor |= (base >> 16) & 0x000000FF;      
    descriptor |=  base        & 0xFF000000;   
 
    descriptor <<= 32;
 
    descriptor |= base  << 16;               
    descriptor |= limit  & 0x0000FFFF;      
    
    return descriptor;
}

void initGdt() {
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint64_t)&gdt_entries;

    gdt_entries[0].core = createDescriptor(0, 0, 0);
    gdt_entries[1].core = createDescriptor(0, 0xFFFFFFFF, GDT_CODE_PL0);
    gdt_entries[2].core = createDescriptor(0, 0xFFFFFFFF, GDT_DATA_PL0);

    gdt_flush(gdt_ptr.base, gdt_ptr.limit);
}

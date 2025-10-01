#include <stdint.h>

struct __attribute__((packed)) gdt_entry_struct {
    uint64_t core;
};

struct __attribute__((packed)) gdt_ptr_struct {
    uint16_t limit;
    uint64_t base;
};


void initGdt();
void setGdtGate(uint32_t num, uint64_t base, uint32_t limit, uint8_t access, uint8_t gran);
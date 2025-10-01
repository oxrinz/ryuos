#include <stdint.h>

struct __attribute__((packed)) idt_ptr_struct
{
    uint16_t limit;
    uint64_t base;
};

struct __attribute__((packed)) idt_entry_struct
{
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
};

__attribute__((noreturn)) void exceptionHandler(void);

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);

void initIdt(void);
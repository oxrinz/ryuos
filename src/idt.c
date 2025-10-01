#include <idt.h>
#include <stdbool.h>
#include <limine.h>
#include <stdint.h>
#include <kernel.h>

#define IDT_MAX_DESCRIPTORS 256

__attribute__((aligned(0x10))) struct idt_entry_struct idt[256];

void exceptionHandler()
{
    asm volatile("cli; hlt");
}

static bool vectors[IDT_MAX_DESCRIPTORS];
static struct idt_ptr_struct idtr;
extern void *isr_stub_table[];

void istSetDescriptor(uint8_t vector, void *isr, uint8_t flags)
{
    struct idt_entry_struct *descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

void pageFaultHandler()
{
    asm volatile("pop %rax");
}

void initIdt()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(struct idt_ptr_struct) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++)
    {
        istSetDescriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    istSetDescriptor(14, &pageFaultHandler, 0x8E);

        asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}

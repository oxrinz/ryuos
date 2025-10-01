global gdt_flush

section .text
gdt_flush:
    mov [gdtr], rsi
    mov [gdtr + 2], rdi
    lgdt [gdtr]

    push 0x08
    lea rax, [rel .reload_cs]
    push rax
    retfq

.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

section .data
gdtr:
    dw 0
    dq 0

BITS 32
section .text
    ; Inizio del bootloader
global _start

section .multiboot
    ; Intestazione Multiboot
    align 4
    dd 0x1BADB002             ; Magic number
    dd 0x00                   ; Flags
    dd -(0x1BADB002 + 0x00)   ; Checksum

section .text
_start:
    cli
    mov esp, stack_top        ; Imposta stack pointer
    
    ; Chiamata al kernel principale
    extern kernel_main
    call kernel_main

    ; Loop infinito in caso di ritorno
.hang:
    jmp .hang

section .bss
    align 4
    stack_bottom:
        resb 16384             ; 16 KB di stack
    stack_top:
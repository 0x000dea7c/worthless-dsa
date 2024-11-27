section .data
    msg db "hello trash", 0xA

section .text
    global _start

_start:
    mov rax, 1                  ; write syscall
    mov rdi, 1                  ; stdout fd
    mov rsi, msg
    mov rdx, 12                 ; bytes to write
    syscall
    mov rax, 60                 ; exit syscall
    mov rdi, 0                  ; exit code is 0, all gudz
    syscall

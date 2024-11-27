section .data
    msg_usage db "usage ./guess [number between 1 and 100]", 0xA
    msg_ok db "you guessed right!!", 0xA
    msg_nok db "you guessed wrong!!", 0xA
    guess equ 50

section .text
    global _start

    ; FIXME: SEGFAULT HERE!!!
base10stoi:
    ; formula: total = total * 10 + digit, stops converting on the
    ; first non digit character
    movzx eax, byte [rdi]       ; start w/ first digit
    sub eax, '0'                ; convert from ASCII to digit
    cmp al, 9                   ; check that it's a decimal
    jbe .loop                   ; start work

    ; bad digit, return 0 and fk u
    xor eax, eax                ; zero out the result
    ret

.next_digit:
    lea eax, [rax * 4 + rax]    ; total *= 5
    lea eax, [rax * 2 + rcx]    ; total = (total * 5) * 2 + digit

.loop:
    inc rdi
    movzx ecx, byte [rdi]
    sub ecx, '0'
    cmp ecx, 9
    jbe .next_digit
    ret                         ; return in eax

_start:
    ; rdi = argc
    ; rsi = argv (pointer to an array of strings)
    pop rax                     ; get argc (1st thing on the stack)
    mov rbx, rsp                ; rbx now points to argv[0]
    cmp rax, 2
    jne _usage
    mov rsi, [rbx + 8]          ; put in rsi argv[1]
    call base10stoi             ; first need to convert the argument to a number
    cmp rax, guess
    je _ok                      ; here if u did good
    jne _nok                    ; here if u suck so much

_ok:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_ok
    mov rdx, 20
    syscall
    jmp _exit

_nok:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_nok
    mov rdx, 20
    syscall
    jmp _exit

_usage:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_usage
    mov rdx, 41
    syscall
    jmp _exit

_exit:
    mov rax, 60                 ; exit syscall
    mov rdi, 0                  ; exit_sucess
    syscall

section .data
    msg_usage db "usage ./guess [number between 1 and 100]", 0xA
    msg_usage_len equ $-msg_usage
    msg_ok db "you guessed right!!", 0xA
    msg_ok_len equ $-msg_ok
    msg_nok db "you guessed wrong!!", 0xA
    msg_nok_len equ $-msg_nok
    msg_err_parse db "Error parsing number", 0xA
    msg_err_parse_len equ $-msg_err_parse
    guess equ 50
    err_code equ -69

section .text
    global _start

base10stoi:
    push rbp
    mov rbp, rsp
    xor rax, rax                ; initalise result to 0
    xor rcx, rcx                ; clear contents of rcx, store the current digit here

_next_digit:
    movzx rcx, byte [rdi]       ; move the contents of rdi (ptr to the string) to rcx
    test cl, cl                 ; null terminator check
    jz _done                    ; jump if zero bc it's null, so end
    cmp cl, '0'                 ; the cl register contains the lower 8 bits of rcx
    jl _error                   ; these two checks are to see if the character is a digit
    cmp cl, '9'
    jg _error
    sub cl, '0'                 ; convert ASCII to digit
    mov rdx, 10
    mul rdx                     ; rax *= 10
    test rdx, rdx               ; check for possible overflow
    jnz _error
    add rax, rcx                ; add this new digit
    jc _error                   ; another check for possible overflow, jc (check for zero in the cx register)
    inc rdi                     ; move to the next byte
    jmp _next_digit

_error:
    mov rax, err_code
    jmp _exit_base10

_done:
                                ; nothing to do here, result is already in rax
_exit_base10:
    pop rbp
    ret

_start:
    ; rdi -> 1st arg
    ; rsi -> 2nd arg
    ; rdx -> 3rd arg
    ; rcx -> 4th arg
    ; r8 -> 5th arg
    ; r9 -> 6th arg
    ; rdi = argc
    ; rsi = argv (pointer to an array of strings)
    pop rax                     ; get argc (1st thing on the stack)
    cmp rax, 2
    jne _usage
    mov rdi, [rsp + 8]          ; first arg is stored in rdi
    call base10stoi             ; first need to convert the argument to a number
    cmp rax, err_code
    je _error_parsing
    cmp rax, guess
    je _ok                      ; here if u did good
    jne _nok                    ; here if u suck so much

_ok:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_ok
    mov rdx, msg_ok_len
    syscall
    jmp _exit

_nok:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_nok
    mov rdx, msg_nok_len
    syscall
    jmp _exit

_error_parsing:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_err_parse
    mov rdx, msg_err_parse_len
    syscall
    jmp _exit

_usage:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg_usage
    mov rdx, msg_usage_len
    syscall
    jmp _exit

_exit:
    mov rax, 60                 ; exit syscall
    mov rdi, 0                  ; exit_sucess
    syscall

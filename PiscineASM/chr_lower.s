section .text
	global chr_lower

chr_lower:
        mov rax, 0x20
        or rax, rdi
        ret

section .note.GNU-stack noalloc noexec nowrite progbits

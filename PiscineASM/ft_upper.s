section .text
	global ft_upper

ft_upper:
        mov rcx, -1
.loop:
        inc rcx
        cmp byte [rdi + rcx], 0
        je .end
        and byte [rdi + rcx], 0xDF
        jmp .loop
.end:
        ret 


section .note.GNU-stack noalloc noexec nowrite progbits

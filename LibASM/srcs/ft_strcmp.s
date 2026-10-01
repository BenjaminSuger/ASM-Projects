section .text
	global ft_strcmp

ft_strcmp:
	mov rdx, -1
.start:
	inc rdx
	movzx eax, byte [rdi + rdx]
	movzx ecx, byte [rsi + rdx]
	cmp al, cl
	jne .end
	test al, al
	jnz .start
.end:
	sub eax, ecx
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

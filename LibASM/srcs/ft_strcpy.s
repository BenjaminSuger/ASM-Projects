section .text
	global ft_strcpy

ft_strcpy:
	mov rcx, -1
.loop:
	inc rcx
	movzx eax, byte [rsi + rcx]
	mov [rdi + rcx], al
	test al, al	
	jne .loop
	mov rax, rdi
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

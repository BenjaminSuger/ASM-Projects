section .text
	global ft_read
	extern __errno_location

ft_read:
	xor eax, eax
	syscall
	test rax, rax
	jge .fin
	neg rax
	push rax
	call  __errno_location wrt ..plt
	pop rdi
	mov [rax], edi
	mov rax, -1
.fin:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

section .text
	global ft_strdup
	extern ft_strlen
	extern ft_strcpy
	extern malloc

ft_strdup:
	push rdi
	call ft_strlen
	inc rax
	mov rdi, rax
	call malloc wrt ..plt
	test rax, rax
	je .end
	mov rdi, rax
	mov rsi, QWORD [rsp]
	call ft_strcpy 
.end:
	pop rdi
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

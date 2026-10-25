%include "list.inc"

section .text
	global ft_list_push_front
	extern malloc

ft_list_push_front:
	test rdi, rdi
	jz .end
	push rdi
	push rsi
	sub rsp, 8
	mov rdi, t_list_size
	call malloc wrt ..plt
	add rsp, 8
	pop rsi
	pop rdi
	test rax, rax
	jz .end
	mov [rax + t_list.data], rsi
	mov rdx, [rdi]
	mov [rax + t_list.next], rdx
	mov [rdi], rax
.end:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

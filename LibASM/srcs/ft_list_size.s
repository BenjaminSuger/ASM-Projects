%include "list.inc"

section .text
	global ft_list_size

ft_list_size:
	mov rcx, 0
	cmp rdi, 0
	je .end
.loop:
	inc rcx
	mov rdi, [rdi + t_list.next]
	cmp rdi, 0
	jnz .loop
.end:
	mov rax, rcx
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

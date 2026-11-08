struc t_list
    .data resq 1
    .next resq 1
endstruc

section .text
	global ft_list_remove_if
	extern free

ft_list_remove_if:
	test rdi, rdi
	jz .over
	push r12
	push r13
	push r14
	push r15
	push rbx ; va me servir pour le node 
	mov rbx, rdi ; t_list **begin_list
	mov r13, rsi ; void *data_ref
	mov r14, rdx ; int (*cmp)()
	mov r15, rcx ; void (*free_fct)(void *)
.loop:
	mov r12, [rbx]
	test r12, r12
	jz .end
	mov rdi, [r12 + t_list.data]
	mov rsi, r13
	call r14
	test rax, rax
	jz .free
	lea rbx, [r12 + t_list.next]
	jmp .loop

.free:
	mov rax, [r12 + t_list.next]
	mov [rbx], rax
	mov rdi, [r12 + t_list.data]
	call r15
	mov rdi, r12
	call free wrt ..plt
	jmp .loop
.end:
	pop rbx
	pop r15
	pop r14
	pop r13
	pop r12
.over:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

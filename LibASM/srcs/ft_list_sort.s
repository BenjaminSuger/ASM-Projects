
%include "list.inc"

section .text
	global ft_list_sort
	extern ft_list_size

ft_list_sort:
	cmp rdi, 0
	je .finish
	cmp rsi, 0
	je .finish
	push r12 ; t_list **begin_list
	push r13 ; int (*cmp)()
	push rbx
	push r14
	mov rbx, 0
	mov r12, rdi
	mov r13, rsi
	mov rdi, [rdi]
	sub rsp, 8
	call ft_list_size
	add rsp, 8 
	cmp rax, 1
	jle .end
	mov rcx, rax
.loop:
	inc rbx
	cmp rcx, rbx
	je .end 
	mov r14, [r12] 
	jmp .pass

.pass: 
	cmp QWORD [r14 + t_list.next], 0
	je .loop
	mov rdi, [r14 + t_list.data]
	mov rsi, [r14 + t_list.next]
	mov rsi, [rsi + t_list.data]
	push rcx
	call r13
	pop rcx
	test eax, eax
	jg .switch
	mov r14, [r14 + t_list.next]
	jmp .pass
.switch: 
	mov rdi, [r14 + t_list.data] 
	mov rsi, [r14 + t_list.next]
	mov rsi, [rsi + t_list.data]
	mov [r14 + t_list.data], rsi
	mov rax, [r14 + t_list.next]
	mov [rax + t_list.data], rdi
	mov r14, [r14 + t_list.next]
	jmp .pass
.end:
	pop r14
	pop rbx
	pop r13
	pop r12
.finish:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

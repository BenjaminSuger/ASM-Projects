section .text
	global ft_write
	global ft_strlen
	global ft_putstr
	extern __errno_location

ft_strlen:
        mov rax, -1
.loop:
        inc rax
        cmp byte [rdi + rax], 0
        jne .loop
        ret

ft_write:
	mov rax, 1 
	syscall
	cmp rax, 0
	jge .fin
	neg rax
	push rax
	call  __errno_location wrt ..plt
	pop rdi
	mov [rax], edi
	mov rax, -1
.fin:
	ret

ft_putstr:
	push rbx
	mov rbx, rdi
	call ft_strlen
	mov rsi, rbx
	mov rdi, 1
	mov rdx, rax
	call ft_write
	pop rbx
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
